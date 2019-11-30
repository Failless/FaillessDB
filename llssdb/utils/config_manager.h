#ifndef FAILLESS_LLSSDB_UTILS_CONFIG_MANAGER_H_
#define FAILLESS_LLSSDB_UTILS_CONFIG_MANAGER_H_

#include "llssdb/utils/settings.h"

namespace failless {
namespace db {
namespace utils {

class ConfigManager {
 public:
    explicit ConfigManager(const char *path);  // path to .conf file
    ~ConfigManager() = default;

    bool Initialize(Settings &settings);

 private:
    const char *config_path;

    virtual void WriteToSettings_(Settings &settings, std::ifstream &cFile);
};

}  // namespace utils
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_UTILS_CONFIG_MANAGER_H_
