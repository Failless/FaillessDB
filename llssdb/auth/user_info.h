//
// Created by sergey on 16.11.2019.
//

#ifndef FAILLESS_USER_INFO_H
#define FAILLESS_USER_INFO_H

#include <string>
#include <utility>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid.hpp>
#include <openssl/sha.h>
// #include <boost/uuid/uuid_io.hpp>


struct UserInfo {
    std::string login;
    unsigned char pass_hash[SHA256_DIGEST_LENGTH]{};
    boost::uuids::uuid client_id{};
    bool is_conn{};
    int table_id{};

    explicit UserInfo(std::string user_login)
            : login(std::move(user_login)),
              client_id(boost::uuids::random_generator()()),
              is_conn(false),
              table_id(-1) {}

    UserInfo() = default;
};

#endif //FAILLESS_USER_INFO_H
