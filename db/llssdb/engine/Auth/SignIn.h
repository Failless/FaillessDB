//
// Created by sergey on 09.11.2019.
//

#ifndef FAILLESS_SIGNIN_H
#define FAILLESS_SIGNIN_H

#include <string>

class SignIn {
public:
    std::string login(std::string login, std::string pass);  // token
    explicit SignIn(std::string directory);
    ~SignIn();
private:
    std::string server_db_conn_;
    std::string login_;
    std::string pass_;
    std::string dir_;
    std::string token_;
    bool send_msg_to_SM();  // how?
    // calling login function
};

#endif //FAILLESS_SIGNIN_H
