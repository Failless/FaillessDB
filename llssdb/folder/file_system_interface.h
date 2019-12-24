#ifndef FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_INTERFACE_H_
#define FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_INTERFACE_H_

#include <boost/noncopyable.hpp>
#include <cstdlib>
#include <map>
#include <string>
#include <unordered_map>
#include "llss3p/enums/operators.h"
#include "llss3p/utils/data.h"
#include "llssdb/folder/in_memory_data.h"

namespace failless {
namespace db {
namespace folder {

class FileSystemInterface : boost::noncopyable {
 public:
    FileSystemInterface() = default;
    virtual ~FileSystemInterface() = default;

    virtual common::enums::response_type Get(const std::string& key,
                                             std::vector<uint8_t>& value_out, size_t& size_out) = 0;
    virtual common::enums::response_type Set(common::utils::Data& data) = 0;
    virtual common::enums::response_type Remove(const std::string& key) = 0;

    virtual void EraseAll() = 0;
    virtual void LoadInMemory(
        std::unordered_map<std::string, failless::db::folder::InMemoryData>& local_storage) = 0;
};

}  // namespace folder
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_FOLDER_FILE_SYSTEM_INTERFACE_H_
