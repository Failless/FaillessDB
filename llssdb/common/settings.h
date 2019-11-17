#ifndef FAILLESS_LLSSDB_COMMON_SETTINGS_H_
#define FAILLESS_LLSSDB_COMMON_SETTINGS_H_

#include <string>

namespace failless {
namespace db {
namespace common {

struct Settings {
    std::string ip;
    int port;
};

}  // namespace common
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_COMMON_SETTINGS_H_
