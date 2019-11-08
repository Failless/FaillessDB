//
// Created by sergey on 09.11.2019.
//

#ifndef FAILLESS_AUTHORIZATION_H
#define FAILLESS_AUTHORIZATION_H

#include <string>
#include "Registration.h"
#include "Disconnector.h"
#include "SignIn.h"

class Authorization {
public:
    bool reg(std::string login, std::string pass);
    bool login(std::string login, std::string pass);
    bool disconnect(std::string login, std::string pass); // (token)
    explicit Authorization(std::string directory);
    ~Authorization();
private:
    std::string server_db_conn_;
    Registration reg_;
    SignIn sign_;
    Disconnector disconnect_;
};
#endif //FAILLESS_AUTHORIZATION_H
