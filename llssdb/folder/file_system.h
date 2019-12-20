#ifndef FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H
#define FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H


#include "llssdb/folder/file_system_interface.h"

#include <cstdlib>
#include <string>
#include <unordered_map>

#include <rocksdb/db.h>
#include <rocksdb/utilities/backupable_db.h>

#include "llss3p/enums/operators.h"
#include "llssdb/folder/in_memory_data.h"

namespace failless::db::folder {

class FileSystem : public FileSystemInterface {
public:
    FileSystem() = default;
    explicit FileSystem(const std::string &folder_path);
    ~FileSystem() override;

    common::enums::response_type Get(const std::string &key, std::vector<uint8_t>& value_out, size_t& size_out) override;
    common::enums::response_type Set(const std::string &key, uint8_t *value_in, size_t size_in) override;
    common::enums::response_type Remove(const std::string &key) override;

    void EraseAll() override;
    void LoadInMemory(std::unordered_map<std::string, InMemoryData>& local_storage) override;
    uint64_t AmountOfKeys();
    bool RestoreFromBackup();
private:
    bool OpenDB_();
    void CloseDB_();
    void BackUp_();

    rocksdb::DB *db_ = nullptr;
    rocksdb::BackupEngine *backup_engine_ = nullptr;

    std::string folder_path_;
    std::string db_path_;
    std::string backup_path_;
    bool is_open_ = false;
};

}  // namespace failless::db::folder


#endif  // FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H
