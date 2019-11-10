//
// Created by sergey on 09.11.2019.
//

#ifndef FAILLESS_AUTHORIZATION_H
#define FAILLESS_AUTHORIZATION_H

#include <string>
#include "llssdb/auth/registration.h"
#include "llssdb/auth/disconnector.h"
#include "llssdb/auth/signin.h"

class IAuthorization {
public:
    virtual bool Reg(std::string login, std::string pass) = 0;
    virtual bool Login(std::string login, std::string pass) = 0;
    virtual bool Disconnect(std::string login, std::string pass) = 0; // (token)
    explicit IAuthorization() = default;
    ~IAuthorization() = default;

    int Test(std::string login, std::string pass) {
        Reg(login, pass);
        return EXIT_SUCCESS;
    }
};


class Authorization : public IAuthorization {
public:
    bool Reg(std::string login, std::string pass) override ;
    bool Login(std::string login, std::string pass) override ;
    bool Disconnect(std::string login, std::string pass) override ; // (token)
    explicit Authorization(std::string directory);
    ~Authorization() = default;
private:
    std::string server_db_conn_;
    // Registration reg_;
    // SignIn sign_;
    // Disconnector disconnect_;
};
#endif //FAILLESS_AUTHORIZATION_H
