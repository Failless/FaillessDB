#ifndef FAILLESS_LLSSDB_COMMON_OPERATORS_H_
#define FAILLESS_LLSSDB_COMMON_OPERATORS_H_

namespace failless {
namespace common {
namespace enums {

enum operators {
    REG = -1,
    SET = 0,
    GET,
    UPDATE,
    DELETE,
    CREATE,
    KILL,
    CONNECT,
    DISCONNECT
};

enum response_type {
    SERVER_ERROR = -2,
    NOT_SET = -1,
    OK = 0,
    EXIST,
    NOT_ALLOWED,
    NOT_FOUND,
    NOT_DONE
};

}  // namespace enums
}  // namespace common
}  // namespace failless

#endif  // FAILLESS_LLSSDB_COMMON_OPERATORS_H_
