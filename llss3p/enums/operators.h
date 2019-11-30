#ifndef FAILLESS_LLSSDB_COMMON_OPERATORS_H_
#define FAILLESS_LLSSDB_COMMON_OPERATORS_H_

namespace failless {
namespace common {
namespace enums {

enum operators {
    SET = 0,
    GET,
    UPDATE,
    DELETE,
    CREATE,
    KILL,
    CONNECT,
    DISCONNECT
};

}  // namespace enums
}  // namespace common
}  // namespace failless

#endif  // FAILLESS_LLSSDB_COMMON_OPERATORS_H_
