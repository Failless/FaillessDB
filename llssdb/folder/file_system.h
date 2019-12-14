#ifndef FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H
#define FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H


#include <boost/noncopyable.hpp>
#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <string>

#include "llss3p/enums/operators.h"
#include "llssdb/folder/file_system_interface.h"
#include "llssdb/folder/in_memory_data.h"

namespace failless::db::folder {

class FileSystem : public FileSystemInterface {
public:
    FileSystem() = default;
    explicit FileSystem(const std::string &db_path);
    ~FileSystem() override;

    common::enums::response_type Get(const std::string &key, uint8_t *value_out, size_t& size_out) override;
    common::enums::response_type Set(const std::string &key, uint8_t *value_in, size_t size_in) override;
    common::enums::response_type Remove(const std::string &key) override;

    void EraseAll(const std::string& db_path) override;
    void LoadInMemory(std::unordered_map<std::string, InMemoryData>& local_storage) override;
    uint64_t AmountOfKeys();
private:
    bool OpenDB_(const std::string &db_path);
    void CloseDB_();

    rocksdb::DB *db_ = nullptr;
    bool is_open_ = false;
};

}  // namespace failless::db::folder


#endif  // FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H
