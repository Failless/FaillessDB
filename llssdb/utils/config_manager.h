#ifndef FAILLESS_LLSSDB_UTILS_CONFIG_MANAGER_H_
#define FAILLESS_LLSSDB_UTILS_CONFIG_MANAGER_H_

#include <llssdb/common/settings.h>

#include <iostream>
#include <fstream>
#include <algorithm>

namespace failless {
namespace db {
namespace utils {

class ConfigManager {
 public:
    ConfigManager() = default;
    ~ConfigManager() = default;
    bool Initialize(common::Settings& settings);
 private:
    virtual void WriteToSettings_(failless::db::common::Settings& settings, std::ifstream &cFile);
};

}  // namespace utils
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_UTILS_CONFIG_MANAGER_H_
