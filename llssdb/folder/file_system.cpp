#include "llssdb/folder/file_system.h"
#include <rocksdb/db.h>
#include <rocksdb/iterator.h>
#include <rocksdb/options.h>
#include <iostream>

namespace failless {
namespace db {
namespace folder {

using namespace rocksdb;

FileSystem::FileSystem(
    const std::string& db_path /*, std::map<std::string, ValueInfo>*& local_storage*/)
    : db_(nullptr) {
    OpenDB_(db_path);
    //    LoadInMemory(local_storage);
}

FileSystem::~FileSystem() { CloseDB_(); }

bool FileSystem::OpenDB_(const std::string& db_path) {
    //    DB* db;
    Options options;

    /// Optimize RocksDB. This is the easiest way to get RocksDB to perform well
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();

    /// Create the DB if it's not already present
    options.create_if_missing = true;

    /// Open DB with default ColumnFamily
    auto status = DB::Open(options, db_path, &db_);
    if (!status.ok()) {
        throw std::logic_error("Failed to open a database");
    }
    return true;
}

void FileSystem::CloseDB_() {
    /// Close db
    if (db_) {
        auto status = db_->Close();
        if (!status.ok()) {
            throw std::logic_error("Failed to close a database");
        }
    }
    delete db_;
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
        }
        return status.ok();
    }
    return false;
}

bool FileSystem::Remove(const std::string& key) {
    /// Remove key
    if (db_) {
        auto status = db_->Delete(WriteOptions(), key);
        if (!status.ok()) {
            std::cerr << "Failed to delete a value\n";  // TODO(EgorBedov): fix that later
        }
        return status.ok();
    }
    return false;
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
    if (db_) {
        uint64_t keys = 0;
        db_->GetAggregatedIntProperty("rocksdb.estimate-num-keys", &keys);
        return keys;
    }
    // TODO(EgorBedov): handle this error
    return -1;
}

void FileSystem::LoadInMemory(std::map<std::string, ValueInfo>*& local_storage) {
    if (db_) {
        // TODO(EgorBedov): map will insert at-runtime-known amount of nodes
        // but there's no way to allocate memory in advance (boost?)
        ReadOptions read_options;
        Iterator* it = db_->NewIterator(read_options);
        ValueInfo temp;
        for (it->SeekToFirst(); it->Valid(); it->Next()) {
            temp.in_memory = true;
            temp.size = it->value().size();
            // next thing is copying data from const char * to int8_t * (seems fine to me tho)
            memcpy(temp.value, it->value().data(), temp.size * sizeof(int8_t));
            local_storage->at(it->key().ToString()) = temp;
        }
    }
}

}  // namespace folder
}  // namespace db
}  // namespace failless

// https://github.com/facebook/rocksdb/blob/master/examples/column_families_example.cc
// TODO(EgorBedov): https://github.com/facebook/rocksdb/wiki/A-Tutorial-of-RocksDB-SST-formats
