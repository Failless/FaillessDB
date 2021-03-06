#ifndef FAILLESS_USER_INFO_H
#define FAILLESS_USER_INFO_H

#include <openssl/sha.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>
#include <utility>
// #include <boost/uuid/uuid_io.hpp>

namespace failless {
namespace db {
namespace auth {

struct UserInfo {
    std::string login;
    unsigned char pass_hash[SHA256_DIGEST_LENGTH]{};
    // boost::uuids::uuid client_id{};
    bool is_conn{};
    int table_id{};

    explicit UserInfo(std::string user_login)
        : login(std::move(user_login)),
          pass_hash(""),
          // client_id(boost::uuids::random_generator()()),
          is_conn(false),
          table_id(-1) {}

    UserInfo() = default;
};

}  // namespace auth
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_USER_INFO_H
