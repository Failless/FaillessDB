#ifndef FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H
#define FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H

#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <boost/noncopyable.hpp>
#include <string>
#include "llssdb/folder/value_info.h"

namespace failless::db::folder {

using std::string;

class FileSystemInterface : boost::noncopyable {
public:
    FileSystemInterface() = default;
    virtual ~FileSystemInterface() = default;

    virtual bool Get(const string &key, std::shared_ptr<int8_t>& value_out, size_t size_out) = 0;
    virtual bool Set(const string &key, std::shared_ptr<int8_t> value_in, size_t size_in) = 0;
//    virtual bool GetRange(const string &key, std::shared_ptr<int8_t> value) = 0;
    virtual bool Remove(const string &key) = 0;
    virtual void EraseAll(const string& db_path) = 0;

protected:
    virtual void LoadInMemory_(std::map<string, ValueInfo>& local_storage) = 0;
};

class FileSystem : public FileSystemInterface {
public:
    explicit FileSystem(const string &db_path);
    ~FileSystem() override;

    bool Get(const string &key, std::shared_ptr<int8_t>& value_out, size_t size_out) override;
    bool Set(const string &key, std::shared_ptr<int8_t> value_in, size_t size_in) override;
//    bool GetRange(const string &key, int8_t* value) override;
    bool Remove(const string &key) override;
    void EraseAll(const string& db_path) override;

    uint64_t AmountOfKeys();
    void LoadInMemory_(std::map<string, ValueInfo>& local_storage) override;
private:
    bool OpenDB_(const string &db_path);
    void CloseDB_();
    rocksdb::DB *db_;
};

}  // namespace failless::db::folder

#endif  // FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H
