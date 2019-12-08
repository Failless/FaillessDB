#include <iostream>
#include <memory>
#include <rocksdb/db.h>
#include <rocksdb/iterator.h>
#include <rocksdb/options.h>
#include <string>

#include "llssdb/folder/file_system.h"

namespace failless::db::folder {

using namespace rocksdb;

FileSystem::FileSystem(const std::string &db_path) {
    OpenDB_(db_path);
}

FileSystem::~FileSystem() {
    CloseDB_();
}

bool FileSystem::OpenDB_(const std::string &db_path) {
    Options options;

    /// Optimize RocksDB. This is the easiest way to get RocksDB to perform well
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();

    /// Create the DB if it's not already present
    options.create_if_missing = true;

    /// Open DB with default ColumnFamily
    Status s = DB::Open(options, db_path, &db_);
    if (!s.ok()) {
        is_open_ = false;
        std::cerr << "Failed to open a database\n";
        return false;
    } else {
        is_open_ = true;
        return true;
    }
}

void FileSystem::CloseDB_() {
    if (is_open_) {
        /// Close db
        is_open_ = false;
        delete db_;
        db_ = nullptr;
    }
}

size_t FileSystem::Get(const std::string &key, uint8_t *value_out) {
    if (is_open_) {
        PinnableSlice pinnable_value;
        auto status = db_->Get(ReadOptions(), db_->DefaultColumnFamily(), key, &pinnable_value);

        if (!status.ok()) {
            return 0;
        }
        /// Copy to output arguments

        size_t size = pinnable_value.size();
        value_out = new uint8_t[size];
        memcpy(value_out, pinnable_value.data(), size * sizeof(decltype(value_out)));
        return size;
    }
    return 0;
}

bool FileSystem::Set(const std::string &key, uint8_t *value_in, size_t size_in) {
    if (is_open_) {
        std::string string_value{};

        for (size_t iii = 0; iii < size_in; ++iii) {
            string_value += std::to_string(value_in[iii]);
        }

        auto status = db_->Put(WriteOptions(), key, string_value);
        if (!status.ok()) {
            std::cerr << "Failed to put a value\n";
            return false;
        }
        return true;
    } else {
        return false;
    }
}

bool FileSystem::Remove(const std::string &key) {
    if (is_open_) {
        /// Find key first
        auto value = new std::string;
        Status s = db_->Get(ReadOptions(), key, value);
        if (s.IsNotFound()) {
            std::cerr << "Such key doesn't exist" << std::endl;
            return false;
        }
        /// Remove key
        s = db_->Delete(WriteOptions(), key);
        if (!s.ok()) {
            std::cerr << "Failed to delete a value" << std::endl;
            return false;
        }
        return true;
    } else {
        return false;
    }
}

void FileSystem::EraseAll(const std::string &db_path) {
    /// First of all close DB, otherwise it's undefined behaviour
    CloseDB_();

    /// Erase everything
    Options options;
    DestroyDB(db_path, options);

    /// Open DB again because we call for this func in destructor
    OpenDB_(db_path);
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

void FileSystem::LoadInMemory(std::map<std::string, InMemoryData> &local_storage) {
    // TODO(EgorBedov): map will insert at-runtime-known amount of nodes
    //  but there's no way to allocate memory in advance (boost?)
    if (is_open_) {
        auto it = db_->NewIterator(ReadOptions());
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
    } else {
        return;
    }
}

}

// https://github.com/facebook/rocksdb/blob/master/examples/column_families_example.cc
// TODO(EgorBedov): https://github.com/facebook/rocksdb/wiki/A-Tutorial-of-RocksDB-SST-formats
