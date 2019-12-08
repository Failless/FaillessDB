#ifndef FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_INTERFACE_H_
#define FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_INTERFACE_H_

#include <boost/noncopyable.hpp>

#include "llssdb/folder/in_memory_data.h"

class FileSystemInterface : boost::noncopyable {
public:
    FileSystemInterface() = default;
    virtual ~FileSystemInterface() = default;

    virtual size_t Get(const std::string &key, uint8_t *value_out) = 0;
    virtual bool Set(const std::string &key, uint8_t *value_in, size_t size_in) = 0;
    virtual bool Remove(const std::string &key) = 0;
    virtual void EraseAll(const std::string& db_path) = 0;

    virtual void LoadInMemory(std::map<std::string, failless::db::folder::InMemoryData>& local_storage) = 0;
};

#endif // FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_INTERFACE_H_
