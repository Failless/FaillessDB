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
    Status s = DB::Open(options, db_path, &db_);
    if (!s.ok()) {
        std::cerr << "Failed to open a database\n";
        return false;
    }
    return true;
}

void FileSystem::CloseDB_() {
    /// Close db
    Status s = db_->Close();
    if (!s.ok()) std::cerr << "Failed to close a database\n";

    delete db_;
}

bool FileSystem::Get(const std::string& key, int8_t*& value_out, size_t size_out) {
    PinnableSlice pinnable_value;
    Status s = db_->Get(ReadOptions(), db_->DefaultColumnFamily(), key, &pinnable_value);

    /// Copy to output arguments
    size_out = pinnable_value.size();
    value_out = new int8_t[size_out];
    memcpy(value_out, pinnable_value.data(), size_out * sizeof(int8_t));

    return s.ok();
}

bool FileSystem::Set(const std::string& key, int8_t* value_in, size_t size_in) {
    std::string string_value = std::to_string(*(value_in));
    if (db_ != nullptr) {
        Status s = db_->Put(WriteOptions(), key, string_value);
        if (!s.ok()) {
            std::cerr << "Failed to put a value\n";
            return false;
        }
    } else {
        return false;
    }
    return true;
}

bool FileSystem::Remove(const std::string& key) {
    /// Remove key
    Status s = db_->Delete(WriteOptions(), key);
    if (!s.ok()) {
        std::cerr << "Failed to delete a value\n";  // TODO(EgorBedov): fix that later
        return false;
    }
    return true;
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
    uint64_t keys = 0;
    db_->GetAggregatedIntProperty("rocksdb.estimate-num-keys", &keys);
    return keys;
}

void FileSystem::LoadInMemory(std::map<std::string, ValueInfo>*& local_storage) {
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

}  // namespace folder
}  // namespace db
}  // namespace failless

// https://github.com/facebook/rocksdb/blob/master/examples/column_families_example.cc
// TODO(EgorBedov): https://github.com/facebook/rocksdb/wiki/A-Tutorial-of-RocksDB-SST-formats
