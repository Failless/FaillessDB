#ifndef FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H
#define FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H

#include <rocksdb/db.h>
#include <boost/noncopyable.hpp>
#include <string>
#include "llssdb/folder/value_info.h"

namespace failless {
namespace db {
namespace folder {

class FileSystemInterface : boost::noncopyable {
 public:
    FileSystemInterface() = default;
    virtual ~FileSystemInterface() = default;

    virtual bool Get(const std::string &key, uint8_t *&value_out, size_t size_out) = 0;
    virtual bool Set(const std::string &key, uint8_t *value_in, size_t size_in) = 0;
    //    virtual bool GetRange(const std::string &key, int8_t* value) = 0;
    virtual bool Remove(const std::string &key) = 0;
    virtual void EraseAll(const std::string &db_path) = 0;
};

class FileSystem : public FileSystemInterface {
 public:
    explicit FileSystem(
        const std::string &db_path /*, std::map<std::string, ValueInfo>*& local_storage*/);
    ~FileSystem() override;

    bool Get(const std::string &key, uint8_t *&value_out, size_t size_out) override;
    bool Set(const std::string &key, uint8_t *value_in, size_t size_in) override;
    //    bool GetRange(const std::string &key, int8_t* value) override;
    bool Remove(const std::string &key) override;
    void EraseAll(const std::string &db_path) override;

    uint64_t AmountOfKeys();
    void LoadInMemory(std::map<std::string, ValueInfo> *&local_storage);

 private:
    bool OpenDB_(const std::string &db_path);
    void CloseDB_();
    rocksdb::DB *db_;
};

}  // namespace folder
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H
