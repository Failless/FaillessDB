#ifndef FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H
#define FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H

#include <boost/noncopyable.hpp>
#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <string>

#include "llssdb/folder/file_system_interface.h"
#include "llssdb/folder/in_memory_data.h"

namespace failless {
namespace db {
namespace folder {

class FileSystem : public FileSystemInterface {
public:
    FileSystem() = default;
    explicit FileSystem(const std::string &db_path);
    ~FileSystem() override;

    size_t Get(const std::string &key, uint8_t *value_out) override;
    bool Set(const std::string &key, uint8_t *value_in, size_t size_in) override;
    bool Remove(const std::string &key) override;
    void EraseAll(const std::string& db_path) override;

    uint64_t AmountOfKeys();
    void LoadInMemory(std::map<std::string, InMemoryData>& local_storage) override;

private:
    bool OpenDB_(const std::string &db_path);
    void CloseDB_();

    rocksdb::DB *db_ = nullptr;
    bool is_open_ = false;
};

}  // namespace folder
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H
