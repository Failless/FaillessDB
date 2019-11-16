//
// Created by sergey on 16.11.2019.
//

#ifndef FAILLESS_IAUTHORIZATION_H
#define FAILLESS_IAUTHORIZATION_H

#include "llssdb/auth/user_info.h"
#include <string>
#include <boost/noncopyable.hpp>
#include <map>


class IAuthorization : boost::noncopyable {
public:
    virtual bool Registration(std::string login, std::string pass) = 0;
    virtual bool SignIn(std::string login, std::string pass) = 0;
    IAuthorization() = default;
    ~IAuthorization() = default;

    // mock_test1
    int Test(std::string login, std::string pass) {
        Registration(std::move(login), std::move(pass));
        return EXIT_SUCCESS;
    }
};

#endif //FAILLESS_IAUTHORIZATION_H
