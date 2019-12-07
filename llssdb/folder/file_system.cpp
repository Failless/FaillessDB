#include "llssdb/folder/file_system.h"
#include <rocksdb/db.h>
#include <rocksdb/iterator.h>
#include <rocksdb/options.h>
#include <iostream>
#include <memory>
#include <string>

namespace failless {
namespace db {
namespace folder {

using namespace rocksdb;

FileSystem::FileSystem(const std::string& db_path) : db_(nullptr) { OpenDB_(db_path); }

FileSystem::~FileSystem() { CloseDB_(); }

bool FileSystem::OpenDB_(const std::string& db_path) {
    Options options;

    /// Optimize RocksDB. This is the easiest way to get RocksDB to perform well
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();

    /// Create the DB if it's not already present
    options.create_if_missing = true;

    /// Open DB with default ColumnFamily
    Status s = DB::Open(options, db_path, &db_);
    if (!s.ok()) {
        std::cerr << "Failed to open a database\n";
        return false;
    }
    return true;
}

void FileSystem::CloseDB_() {
    if (DBIsOpen_()) {
        /// Close db
        Status s = db_->Close();
        if (!s.ok())
            std::cerr << "Failed to close a database\n"
                      << "If you're debugging then goto: tests/tests_db/mocks.h" << std::endl;

        delete db_;
        db_ = nullptr;
    }
}

size_t FileSystem::Get(const std::string& key, uint8_t* value_out) {
    if (db_) {
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

bool FileSystem::Set(const std::string& key, uint8_t* value_in, size_t size_in) {
    if (db_) {
        // TODO(EgorBedov): !critical! fix byte to string cast
        std::string string_value = std::to_string(*value_in);
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

bool FileSystem::Remove(const std::string& key) {
    if (DBIsOpen_()) {
        /// Find key first
        std::string* value;
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

void FileSystem::EraseAll(const std::string& db_path) {
    /// First of all close DB, otherwise it's undefined behaviour
    CloseDB_();

    /// Erase everything
    Options options;
    DestroyDB(db_path, options);

    /// Open DB again because we call for this func in destructor
    OpenDB_(db_path);
}

uint64_t FileSystem::AmountOfKeys() {
    if (DBIsOpen_()) {
        uint64_t keys = 0;
        db_->GetAggregatedIntProperty("rocksdb.estimate-num-keys", &keys);
        return keys;
    } else {
        return 0;
    }
}

void FileSystem::LoadInMemory(std::map<std::string, ValueInfo>& local_storage) {
    // TODO(EgorBedov): map will insert at-runtime-known amount of nodes
    //  but there's no way to allocate memory in advance (boost?)
    if (DBIsOpen_()) {
        ReadOptions read_options;
        Iterator* it = db_->NewIterator(read_options);
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            ValueInfo tmp(nullptr, it->value().size(), true);
            memcpy(tmp.value, it->value().data(), tmp.size * sizeof(uint8_t));
            local_storage.at(it->key().ToString()) = tmp;
        }
    } else {
        return;
    }
}

bool FileSystem::DBIsOpen_() {
    if (!db_) {
        std::cerr << "DB wasn't opened" << std::endl;
        return false;
    }
    return true;
}

}  // namespace folder
}  // namespace db
}  // namespace failless

// https://github.com/facebook/rocksdb/blob/master/examples/column_families_example.cc
// TODO(EgorBedov): https://github.com/facebook/rocksdb/wiki/A-Tutorial-of-RocksDB-SST-formats
