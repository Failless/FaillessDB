#ifndef FAILLESS_LLSS3P_UTILS_PACKET_H_
#define FAILLESS_LLSS3P_UTILS_PACKET_H_

#include <string>
#include "llss3p/utils/data.h"


namespace failless {
namespace common {
namespace utils {

struct Packet {
    std::string login;
    std::string pass;
    std::string request;
    Data data;
};

}  // namespace utils
}  // namespace common
}  // namespace failless

#endif  // FAILLESS_LLSS3P_UTILS_PACKET_H_
