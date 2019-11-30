#ifndef FAILLESS_LLSSDB_UTILS_SETTINGS_H_
#define FAILLESS_LLSSDB_UTILS_SETTINGS_H_

#include <string>

namespace failless {
namespace db {
namespace utils {

struct Settings {
    std::string ip;
    int port;
};

}  // namespace utils
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_UTILS_SETTINGS_H_
