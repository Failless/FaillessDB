#include <iostream>
#include <memory>
#include <rocksdb/db.h>
#include <rocksdb/iterator.h>
#include <rocksdb/options.h>
#include <string>
#include "llssdb/folder/file_system.h"

namespace failless::db::folder {

using namespace failless::db::folder;
using namespace rocksdb;


FileSystem::FileSystem(const std::string& db_path) : db_(nullptr) {
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
    if ( !s.ok() ) {
        std::cerr << "Failed to open a database\n";
        return false;
    }
    return true;
}

void FileSystem::CloseDB_() {
    if ( DBIsOpen_() ) {
        /// Close db
        Status s = db_->Close();
        if ( !s.ok() )
            std::cerr << "Failed to close a database\n" <<
            "If you're debugging then goto: tests/tests_db/mocks.h" << std::endl;

        delete db_;
        db_ = nullptr;
    }
}

bool FileSystem::Get(const std::string &key, std::shared_ptr<int8_t>& value_out, size_t& size_out) {
    if ( DBIsOpen_() ) {
        PinnableSlice pinnable_value;
        Status s = db_->Get(ReadOptions(), db_->DefaultColumnFamily(), key, &pinnable_value);

        /// Copy to output arguments
        size_out = pinnable_value.size();
        value_out.reset();
        value_out = std::shared_ptr<int8_t>(
            new int8_t[size_out],
            std::default_delete<int8_t[]>());
        memcpy(value_out.get(), pinnable_value.data(), size_out * sizeof(int8_t));

        return s.ok();
    } else {
        value_out = nullptr;
        return false;
    }
}

bool FileSystem::Set(const std::string &key, const std::shared_ptr<int8_t>& value_in, size_t size_in) {
    if ( DBIsOpen_() ) {
        std::string string_value;

        for ( size_t iii = 0; iii < size_in; ++iii ) {
            string_value += std::to_string(value_in.get()[iii]);
        }

        Status s = db_->Put(WriteOptions(), key, string_value);
        if ( !s.ok() ) {
            std::cerr << "Failed to put a value\n";
            return false;
        }
        return true;
    } else {
        return false;
    }
}

bool FileSystem::Remove(const std::string& key) {
    if ( DBIsOpen_() ) {
        /// Find key first
        std::string* value;
        Status s = db_->Get(ReadOptions(), key, value);
        if ( s.IsNotFound() ) {
            std::cerr << "Such key doesn't exist" << std::endl;
            return false;
        }
        /// Remove key
        s = db_->Delete(WriteOptions(), key);
        if ( !s.ok() ) {
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
    if ( DBIsOpen_() ) {
        uint64_t keys = 0;
        db_->GetAggregatedIntProperty("rocksdb.estimate-num-keys", &keys);
        return keys;
    } else {
        return 0;
    }
}

void FileSystem::LoadInMemory_(std::map<std::string, ValueInfo>& local_storage) {
    // TODO(EgorBedov): map will insert at-runtime-known amount of nodes
    //  but there's no way to allocate memory in advance (boost?)
    if ( DBIsOpen_() ) {
        ReadOptions read_options;
        Iterator* it = db_->NewIterator(read_options);
        for ( it->SeekToFirst(); it->Valid(); it->Next() ) {
            ValueInfo tmp(nullptr, it->value().size(), true);

            memcpy(tmp.value.get(), it->value().data(), tmp.size * sizeof(int8_t));
            local_storage.at(it->key().ToString()) = tmp;
        }
    } else {
        return;
    }
}

bool FileSystem::DBIsOpen_() {
    if ( !db_ ) {
        std::cerr << "DB wasn't opened" << std::endl;
        return false;
    }
    return true;
}

}

// https://github.com/facebook/rocksdb/blob/master/examples/column_families_example.cc
// TODO(EgorBedov): https://github.com/facebook/rocksdb/wiki/A-Tutorial-of-RocksDB-SST-formats
