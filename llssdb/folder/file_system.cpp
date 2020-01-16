#include "llssdb/folder/file_system.h"

#include <cstdlib>
#include <string>
#include <unordered_map>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <rocksdb/db.h>
#include <rocksdb/iterator.h>
#include <rocksdb/options.h>
#include <rocksdb/utilities/backupable_db.h>

#include "llss3p/enums/operators.h"
#include "llssdb/utils/cache.h"

#define BACKUPS 1

namespace failless {
namespace db {
namespace folder {

FileSystem::FileSystem(const std::string &folder_path, bool do_backup)
      : folder_path_(folder_path),
        db_path_(folder_path + "/db/"),
        backup_path_(folder_path + "/backup/"),
        do_backup_(do_backup)
        {
            if ( OpenDB_() && do_backup_ ) BackUp_();
        }

FileSystem::~FileSystem() {
    CloseDB_();
}

bool FileSystem::OpenDB_() {
    rocksdb::Options options;

    /// Optimize RocksDB. This is the easiest way to get RocksDB to perform well
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();

    /// Create_ the DB if it's not already present
    options.create_if_missing = true;

    /// Open DB with default ColumnFamily
    rocksdb::Status s = rocksdb::DB::Open(options, db_path_, &db_);
    if (!s.ok()) {
        is_open_ = false;
        BOOST_LOG_TRIVIAL(error) << "[FS]: Failed to open db at " << db_path_;
    } else {
        is_open_ = true;
        BOOST_LOG_TRIVIAL(info) << "[FS]: Successfully opened db at " << db_path_;
    }
    return is_open_;
}

void FileSystem::CloseDB_() {
    if (is_open_) {
        if ( do_backup_ ) {
            backup_engine_->PurgeOldBackups(BACKUPS);
            delete backup_engine_;
        }
        delete db_;
        is_open_ = false;
        BOOST_LOG_TRIVIAL(info) << "[FS]: Closed db at " << folder_path_;
    }
}

void FileSystem::BackUp_() {
    rocksdb::Status s = rocksdb::BackupEngine::Open(rocksdb::Env::Default(), rocksdb::BackupableDBOptions(backup_path_), &backup_engine_);
    if (s.ok()) {
        s = backup_engine_->CreateNewBackup(db_);
        if (s.ok()) {
            BOOST_LOG_TRIVIAL(debug) << "[FS]: Backed up db at " << backup_path_;
        } else {
            BOOST_LOG_TRIVIAL(error) << "[FS]: Failed to backup db at " << backup_path_;
        }
    } else {
        BOOST_LOG_TRIVIAL(error) << "[FS]: Failed to start BackupEngine at " << backup_path_;
    }
}

bool FileSystem::RestoreFromBackup() {
    rocksdb::BackupEngineReadOnly* r_backup_engine;
    rocksdb::Status s = rocksdb::BackupEngineReadOnly::Open(rocksdb::Env::Default(), rocksdb::BackupableDBOptions(backup_path_), &r_backup_engine);
    if (s.ok()) {
        r_backup_engine->RestoreDBFromLatestBackup(db_path_, db_path_);
        BOOST_LOG_TRIVIAL(info) << "[FS]: Successfully restored db at " << db_path_;
    } else {
        BOOST_LOG_TRIVIAL(error) << "[FS]: Failed to restore db at " << db_path_;
    }
    delete r_backup_engine;
    return s.ok();
}

common::enums::response_type FileSystem::Get(const std::string &key, std::vector<uint8_t>& value_out, size_t& size_out) {
    if (is_open_) {
        rocksdb::PinnableSlice pinnable_value;
        auto status = db_->Get(rocksdb::ReadOptions(), db_->DefaultColumnFamily(), key, &pinnable_value);

        if ( status.IsNotFound() ) {
            BOOST_LOG_TRIVIAL(error) << "[FS]: Key \"" << key << "\" doesn't exist";
            return common::enums::response_type::NOT_FOUND;
        }

        /// Copy to output arguments
        size_out = pinnable_value.size();
        value_out = std::vector<uint8_t>(pinnable_value.data()[0], pinnable_value.data()[size_out - 1]);
        BOOST_LOG_TRIVIAL(debug) << "[FS]: \"" << key << "\" retrieved from HDD";
        return common::enums::response_type::OK;
    } else {
        BOOST_LOG_TRIVIAL(error) << "[FS]: DB isn't open";
        return common::enums::response_type::SERVER_ERROR;
    }
}

common::enums::response_type FileSystem::Set(common::utils::Data& data) {
    if (is_open_) {
        std::string string_value{};

        for ( const auto& byte: data.value ) {
            string_value += std::to_string(byte);
        }

        auto status = db_->Put(rocksdb::WriteOptions(), data.key, string_value);
        if (!status.ok()) {
            BOOST_LOG_TRIVIAL(error) << "[FS]: Value of size " << data.size << " was not loaded into HDD";
            return common::enums::response_type::SERVER_ERROR;
        }
        BOOST_LOG_TRIVIAL(debug) << "[FS]: Value of size " << data.size << " was loaded into HDD";
        return common::enums::response_type::OK;
    } else {
        BOOST_LOG_TRIVIAL(error) << "[FS]: DB isn't open";
        return common::enums::response_type::SERVER_ERROR;
    }
}

common::enums::response_type FileSystem::Remove(const std::string &key) {
    if (is_open_) {
        /// Find key first
        auto status = db_->Get(rocksdb::ReadOptions(), key, new std::string);

        if (status.IsNotFound()) {
            BOOST_LOG_TRIVIAL(error) << "[FS]: Key \"" << key << "\" doesn't exist in db";
            return common::enums::response_type::NOT_FOUND;
        }

        /// Remove key
        status = db_->Delete(rocksdb::WriteOptions(), key);
        if (!status.ok()) {
            BOOST_LOG_TRIVIAL(error) << "[FS]: Failed to delete a key \"" << key << "\"";
            return common::enums::response_type::SERVER_ERROR;
        }
        BOOST_LOG_TRIVIAL(debug) << "[FS]: Key \"" << key << "\" erased from HDD";
        return common::enums::response_type::OK;
    } else {
        BOOST_LOG_TRIVIAL(error) << "[FS]: DB isn't open";
        return common::enums::response_type::SERVER_ERROR;
    }
}

void FileSystem::EraseAll() {
    BOOST_LOG_TRIVIAL(info) << "[FS]: Erasing db";
    /// First of all close DB, otherwise it's undefined behaviour
    CloseDB_();

    /// Erase everything
    DestroyDB(db_path_, rocksdb::Options());

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

// This func meant to be called only when cache is empty
void FileSystem::LoadCache(utils::cache &cache) {
    if ( is_open_ ) {
        auto it = db_->NewIterator(rocksdb::ReadOptions());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            std::string tmp_key = it->key().ToString();
            size_t tmp_size = it->value().size();

            if (cache.cur_size + tmp_size + tmp_key.size() < cache.max_size) {
                std::vector<uint8_t> tmp_vector(it->value().data_, it->value().data_ + tmp_size);
                cache.insert(tmp_key, tmp_vector);
            } else {
                break;
            }
        }
    }
    auto cur_size = cache.cur_size;
    if ( cur_size < 1024 ) {
        BOOST_LOG_TRIVIAL(info) << "[FS]: " << cur_size << " bytes was loaded into RAM";
    } else if ( cur_size > 1024 && cur_size < 1048576 ) {
        BOOST_LOG_TRIVIAL(info) << "[FS]: " << cur_size / 1024 << "MB was loaded into RAM";
    } else {
        BOOST_LOG_TRIVIAL(info) << "[FS]: " << cur_size / 1048576 << "GB was loaded into RAM";
    }
}

}  // namespace folder
}  // namespace db
}  // namespace failless

// https://github.com/facebook/rocksdb/blob/master/examples/column_families_example.cc
// TODO(EgorBedov): https://github.com/facebook/rocksdb/wiki/A-Tutorial-of-RocksDB-SST-formats
