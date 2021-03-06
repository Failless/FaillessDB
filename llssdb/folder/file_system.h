#ifndef FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H
#define FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H


#include "llssdb/folder/file_system_interface.h"

#include <cstdlib>
#include <string>
#include <unordered_map>

#include <boost/date_time.hpp>
#include <rocksdb/db.h>
#include <rocksdb/utilities/backupable_db.h>

#include "llss3p/enums/operators.h"
#include "llss3p/utils/data.h"
#include "llssdb/folder/in_memory_data.h"

namespace failless {
namespace db {
namespace folder {

class FileSystem : public FileSystemInterface {
public:
    FileSystem() = default;
    explicit FileSystem(const std::string &folder_path, bool do_backup = false);
    ~FileSystem() override;

    common::enums::response_type
    Get(const std::string &key, std::vector<uint8_t> &value_out, size_t &size_out) override;
    common::enums::response_type Set(common::utils::Data &data) override;
    common::enums::response_type Remove(const std::string &key) override;

    void EraseAll() override;
    void LoadCache(
            std::unordered_map<std::string, InMemoryData> &local_storage,
            std::map<boost::posix_time::ptime, std::string> &queue,
            long max_bytes,
            long &cur_bytes) override;

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
    bool do_backup_ = false;
};

}  // namespace folder
}  // namespace db
}  // namespace failless


#endif  // FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_H
