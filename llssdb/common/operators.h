#ifndef FAILLESS_LLSSDB_COMMON_OPERATORS_H_
#define FAILLESS_LLSSDB_COMMON_OPERATORS_H_

namespace failless {
namespace db {
namespace common {

enum operators {
    SET = 0,
    GET,
    UPDATE,
    DELETE,
    CREATE,
    KILL,
    CONNECT,
    DISCONNECT,
    ANSWER
};

}  // namespace common
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_COMMON_OPERATORS_H_
