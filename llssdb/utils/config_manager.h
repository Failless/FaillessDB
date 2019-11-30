#ifndef FAILLESS_LLSSDB_UTILS_CONFIG_MANAGER_H_
#define FAILLESS_LLSSDB_UTILS_CONFIG_MANAGER_H_

#include "llssdb/utils/settings.h"

namespace failless {
namespace db {
namespace utils {

// This is an empty class which I used to imagine how is worked our program (rowbotman)
// TODO (Serjio2888): rewrite this class to smth normal
class ConfigManager {
 public:
    ConfigManager() = default;
    ~ConfigManager() = default;
    void Initialize(Settings& settings);
};

}  // namespace utils
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_UTILS_CONFIG_MANAGER_H_
