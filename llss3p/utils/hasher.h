#ifndef FAILLESS_LLSS3P_UTILS_HASHER_H
#define FAILLESS_LLSS3P_UTILS_HASHER_H

#include <string>

namespace failless {
namespace common {
namespace utils {

bool SimpleSHA256(const std::string &input, unsigned char *md);

}  // namespace utils
}  // namespace common
}  // namespace failless

#endif  // FAILLESS_LLSS3P_UTILS_HASHER_H
