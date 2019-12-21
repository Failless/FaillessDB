#include "llssdb/folder/file_system.h"

#include <cstdlib>
#include <string>
#include <unordered_map>

#include <boost/filesystem.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <rocksdb/db.h>
#include <rocksdb/iterator.h>
#include <rocksdb/options.h>
#include <rocksdb/utilities/backupable_db.h>

#include "llss3p/enums/operators.h"

#define BACKUPS 2

namespace failless::db::folder {

using namespace rocksdb;
using namespace boost::filesystem;
using common::enums::response_type;

FileSystem::FileSystem(const std::string &folder_path)
      : folder_path_(folder_path),
        db_path_(folder_path + "/db/"),
        backup_path_(folder_path + "/backup/")
        {
            OpenDB_();
        }

FileSystem::~FileSystem() {
    CloseDB_();
}

bool FileSystem::OpenDB_() {
    Options options;

    /// Optimize RocksDB. This is the easiest way to get RocksDB to perform well
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();

    /// Create_ the DB if it's not already present
    options.create_if_missing = true;

    /// Open DB with default ColumnFamily
    Status s = DB::Open(options, db_path_, &db_);
    if (!s.ok()) {
        is_open_ = false;
        BOOST_LOG_TRIVIAL(error) << "Failed to open db at " << db_path_;
    } else {
        is_open_ = true;
        BOOST_LOG_TRIVIAL(info) << "Successfully opened db at " << db_path_;
        BackUp_();
    }
    return is_open_;
}

void FileSystem::CloseDB_() {
    if (is_open_) {
        backup_engine_->PurgeOldBackups(BACKUPS);
        is_open_ = false;
        delete db_;
        delete backup_engine_;
        BOOST_LOG_TRIVIAL(info) << "Closed db at " << folder_path_;
    }
}

void FileSystem::BackUp_() {
    Status s = BackupEngine::Open(Env::Default(), BackupableDBOptions(backup_path_), &backup_engine_);
    if (s.ok()) {
        s = backup_engine_->CreateNewBackup(db_);
        if (s.ok()) {
            BOOST_LOG_TRIVIAL(debug) << "Backed up db at " << backup_path_;
        } else {
            BOOST_LOG_TRIVIAL(error) << "Failed to backup db at " << backup_path_;
        }
    } else {
        BOOST_LOG_TRIVIAL(error) << "Failed to start BackupEngine at " << backup_path_;
    }
}

bool FileSystem::RestoreFromBackup() {
    BackupEngineReadOnly* r_backup_engine;
    Status s = BackupEngineReadOnly::Open(Env::Default(), BackupableDBOptions(backup_path_), &r_backup_engine);
    if (s.ok()) {
        r_backup_engine->RestoreDBFromLatestBackup(db_path_, db_path_);
        BOOST_LOG_TRIVIAL(info) << "Successfully restored db at " << db_path_;
    } else {
        BOOST_LOG_TRIVIAL(error) << "Failed to restore db at " << db_path_;
    }
    delete r_backup_engine;
    return s.ok();
}

response_type FileSystem::Get(const std::string &key, std::vector<uint8_t>& value_out, size_t& size_out) {
    if (is_open_) {
        PinnableSlice pinnable_value;
        auto status = db_->Get(ReadOptions(), db_->DefaultColumnFamily(), key, &pinnable_value);

        if ( status.IsNotFound() ) {
            BOOST_LOG_TRIVIAL(error) << "Key \"" << key << "\" doesn't exist";
            return response_type::NOT_FOUND;
        }

        /// Copy to output arguments
        size_out = pinnable_value.size();
        value_out = std::vector<uint8_t>(pinnable_value.data()[0], pinnable_value.data()[size_out - 1]);
        BOOST_LOG_TRIVIAL(debug) << "\"" << key << "\" retrieved from HDD";
        return response_type::OK;
    } else {
        BOOST_LOG_TRIVIAL(error) << "DB isn't open";
        return response_type::SERVER_ERROR;
    }
}

response_type FileSystem::Set(common::utils::Data& data) {
    if (is_open_) {
        std::string string_value{};

        for ( const auto& byte: data.value ) {
            string_value += std::to_string(byte);
        }

        auto status = db_->Put(WriteOptions(), data.key, string_value);
        if (!status.ok()) {
            BOOST_LOG_TRIVIAL(error) << "Value of size " << data.size << " was not loaded into HDD";
            return response_type::SERVER_ERROR;
        }
        BOOST_LOG_TRIVIAL(debug) << "Value of size " << data.size << " was loaded into HDD";
        return response_type::OK;
    } else {
        BOOST_LOG_TRIVIAL(error) << "DB isn't open";
        return response_type::SERVER_ERROR;
    }
}

response_type FileSystem::Remove(const std::string &key) {
    if (is_open_) {
        /// Find key first
        auto status = db_->Get(ReadOptions(), key, new std::string);

        if (status.IsNotFound()) {
            BOOST_LOG_TRIVIAL(error) << "Key \"" << key << "\" doesn't exist in db";
            return response_type::NOT_FOUND;
        }

        /// Remove key
        status = db_->Delete(WriteOptions(), key);
        if (!status.ok()) {
            BOOST_LOG_TRIVIAL(error) << "Failed to delete a key \"" << key << "\"";
            return response_type::SERVER_ERROR;
        }
        BOOST_LOG_TRIVIAL(debug) << "Key \"" << key << "\" erased from HDD";
        return response_type::OK;
    } else {
        BOOST_LOG_TRIVIAL(error) << "DB isn't open";
        return response_type::SERVER_ERROR;
    }
}

void FileSystem::EraseAll() {
    BOOST_LOG_TRIVIAL(info) << "Erasing db";
    /// First of all close DB, otherwise it's undefined behaviour
    CloseDB_();

    /// Erase everything
    DestroyDB(db_path_, Options());

    /// Open DB again because we call for this func in destructor
    OpenDB_();
}

uint64_t FileSystem::AmountOfKeys() {
    if (is_open_) {
        uint64_t keys = 0;
        db_->GetAggregatedIntProperty("rocksdb.estimate-num-keys", &keys);
        return keys;
    } else {
        return 0;
    }
}

void FileSystem::LoadInMemory(std::unordered_map<std::string, InMemoryData> &local_storage) {
    bool all = true;
    if (is_open_) {
        auto it = db_->NewIterator(ReadOptions());
        local_storage.reserve(AmountOfKeys());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            auto tmp_vector = std::vector<uint8_t>(it->value()[0], it->value()[it->value().size() - 1]);
            tmp_vector.shrink_to_fit();
            all = std::min(all, local_storage.emplace(std::make_pair(
                    it->key().ToString(),
                    InMemoryData(
                            tmp_vector,
                            it->value().size(),
                            true))).second);
        }
    }
    if (all) {
        BOOST_LOG_TRIVIAL(info) << "Everything was loaded into RAM";
    } else {
        BOOST_LOG_TRIVIAL(info) << "Not everything was loaded into RAM";
    }
}

}

// https://github.com/facebook/rocksdb/blob/master/examples/column_families_example.cc
// TODO(EgorBedov): https://github.com/facebook/rocksdb/wiki/A-Tutorial-of-RocksDB-SST-formats
