#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <string>

#include "llssdb/folder/file_system.h"

namespace failless::db::folder {

using namespace failless::db::folder;
using namespace rocksdb;
using std::string;


FileSystem::FileSystem(const string& db_path) {
//    DB* db;
    rocksdb::Options options;

    // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();

    // Create the DB if it's not already present
    options.create_if_missing = true;

    // Open DB
    Status s = DB::Open(options, db_path, &db_);
    if ( !s.ok() ) fprintf(stderr, "Failed to open a database"); // TODO(EgorBedov): fix that later
}

FileSystem::~FileSystem() {
    Status s = db_->Close();
    if ( !s.ok() ) fprintf(stderr, "Failed to open a database"); // TODO(EgorBedov): fix that later
    delete db_;
}

bool FileSystem::Get(const string& key) {
    /// Check if db is open
    // Status s = DB::Open(options, test_db_path, &db_);
    /// Get value by key
    string string_value;
    Slice slice_key = key;
    Status s = db_->Get(ReadOptions(), slice_key, &string_value);

    return s.ok();
    /// Put in shared queue
}

bool FileSystem::Set(const string& key, const int8_t* value) {
    /// Check if db is open
    /// Find value by key
//    std::string str=boost::lexical_cast<std::string, int>(i);

//    string string_value = std::to_string(value);
    Slice slice_key = key;
    rocksdb::PinnableSlice slice;
    rocksdb::ColumnFamilyHandle* column_family_handle = nullptr;

//    Status s = db_->Get(ReadOptions(), column_family_handle, slice_key, &slice);
//    if ( !s.ok() ) {
//        fprintf(stderr, "Failed to find a key-value pair"); // TODO(EgorBedov): fix that later
//        return false;
//    }

//    /// Put value by key
//    s = db_->Put(WriteOptions(), slice_key, string_value);
//    if ( !s.ok() ) {
//        fprintf(stderr, "Failed to put a value"); // TODO(EgorBedov): fix that later
//        return false;
//    }

    return true;
}

bool FileSystem::GetRange(const string& key) {
    // Call RocksDB
    // Put data in shared queue
    return true;
}

bool FileSystem::Remove(const string& key) {
    /// Check if db is open

    /// Find key
    string string_value;
    Slice slice_key = key;
    Status s = db_->Get(ReadOptions(), slice_key, &string_value);
    if ( !s.ok() ) {
        fprintf(stderr, "Failed to find a key-value pair"); // TODO(EgorBedov): fix that later
        return false;
    }
    /// Remove key
    s = db_->Delete(WriteOptions(), slice_key);
    if ( !s.ok() ) {
        fprintf(stderr, "Failed to delete a value"); // TODO(EgorBedov): fix that later
        return false;
    }

    return true;
}

}
