//
// Created by sergey on 16.11.2019.
//

#ifndef FAILLESS_USER_INFO_H
#define FAILLESS_USER_INFO_H

#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
// #include <boost/uuid/uuid_io.hpp>


struct UserInfo {
    std::string login;
    std::string pass_hash;
    boost::uuids::uuid client_id;
    bool is_conn;
    int table_id;

    UserInfo() : client_id(boost::uuids::random_generator()()), is_conn(false), table_id(-1) {}
};

#endif //FAILLESS_USER_INFO_H
