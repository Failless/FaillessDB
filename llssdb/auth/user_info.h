//
// Created by sergey on 16.11.2019.
//

#ifndef FAILLESS_USER_INFO_H
#define FAILLESS_USER_INFO_H

#include <string>

struct UserInfo {
    std::string login;
    std::string pass_hash;
    int client_id;
    int dir_id;
    bool is_conn;

    UserInfo() : client_id(0), dir_id(0), is_conn(false) {}
};

#endif //FAILLESS_USER_INFO_H
