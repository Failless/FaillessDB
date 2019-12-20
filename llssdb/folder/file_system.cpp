#include "llssdb/folder/file_system.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>

#include <boost/filesystem.hpp>
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
        std::cerr << "Failed to open a db at " << db_path_ << std::endl;
    } else {
        is_open_ = true;
        std::cout << "Successfully opened a db at " << db_path_ << std::endl;
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
    }
}

void FileSystem::BackUp_() {
    Status s = BackupEngine::Open(Env::Default(), BackupableDBOptions(backup_path_), &backup_engine_);
    if (s.ok()) {
        s = backup_engine_->CreateNewBackup(db_);
        (s.ok() ? std::cout << "Backup-ed " : std::cerr << "Failed to backup ") << backup_path_ << std::endl;
    } else {
        std::cerr << "Failed to start BackupEngine" << std::endl;
    }
}

bool FileSystem::RestoreFromBackup() {
    BackupEngineReadOnly* r_backup_engine;
    Status s = BackupEngineReadOnly::Open(Env::Default(), BackupableDBOptions(backup_path_), &r_backup_engine);
    if (s.ok()) {
        r_backup_engine->RestoreDBFromLatestBackup(db_path_, db_path_);
        std::cout << "Successfully restored db on " << folder_path_ << std::endl;
    } else {
        std::cerr << "Failed to restore db on" << folder_path_ << std::endl;
    }
    delete r_backup_engine;
    return s.ok();
}

response_type FileSystem::Get(const std::string &key, uint8_t *value_out, size_t& size_out) {
    if (is_open_) {
        PinnableSlice pinnable_value;
        auto status = db_->Get(ReadOptions(), db_->DefaultColumnFamily(), key, &pinnable_value);

        if ( status.IsNotFound() ) {
            std::cerr << "Such key doesn't exist" << std::endl;
            return response_type::NOT_FOUND;
        }

        /// Copy to output arguments
        size_out = pinnable_value.size();
        value_out = new uint8_t[size_out];
        memcpy(value_out, pinnable_value.data(), size_out * sizeof(decltype(value_out)));
        return response_type::OK;
    } else {
        return response_type::SERVER_ERROR;
    }
}

response_type FileSystem::Set(const std::string &key, uint8_t *value_in, size_t size_in) {
    if (is_open_) {
        std::string string_value{};

        for (size_t iii = 0; iii < size_in; ++iii) {
            string_value += std::to_string(value_in[iii]);
        }

        auto status = db_->Put(WriteOptions(), key, string_value);
        if (!status.ok()) {
            std::cerr << "Failed to put a value\n";
            return response_type::SERVER_ERROR;
        }
        return response_type::OK;
    } else {
        return response_type::SERVER_ERROR;
    }
}

response_type FileSystem::Remove(const std::string &key) {
    if (is_open_) {
        /// Find key first
        auto value = new std::string;
        auto status = db_->Get(ReadOptions(), key, value);

        if (status.IsNotFound()) {
            std::cerr << "Such key doesn't exist" << std::endl;
            return response_type::NOT_FOUND;
        }

        /// Remove key
        status = db_->Delete(WriteOptions(), key);
        if (!status.ok()) {
            std::cerr << "Failed to delete a value" << std::endl;
            return response_type::SERVER_ERROR;
        }
        return response_type::OK;
    } else {
        return response_type::SERVER_ERROR;
    }
}

void FileSystem::EraseAll() {
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
    if (is_open_) {
        auto it = db_->NewIterator(ReadOptions());
        local_storage.reserve(AmountOfKeys());
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            auto tmp_vector = std::vector<uint8_t>(it->value()[0], it->value()[it->value().size() - 1]);
            tmp_vector.shrink_to_fit();
            local_storage.emplace(std::make_pair(
                    it->key().ToString(),
                    InMemoryData(
                            tmp_vector,
                            it->value().size(),
                            true)));
        }
    }
}

}

// TODO(EgorBedov): fix headers https://google.github.io/styleguide/cppguide.html#Names_and_Order_of_Includes

// https://github.com/facebook/rocksdb/blob/master/examples/column_families_example.cc
// TODO(EgorBedov): https://github.com/facebook/rocksdb/wiki/A-Tutorial-of-RocksDB-SST-formats
