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

    virtual bool IsAuth(std::string login, std::string pass, int table_id) = 0;

    virtual bool RemoveUser(std::string login, std::string pass) = 0;

    virtual ~IAuthorization() = default;
};

#endif //FAILLESS_IAUTHORIZATION_H
