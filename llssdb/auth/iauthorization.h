#ifndef FAILLESS_IAUTHORIZATION_H
#define FAILLESS_IAUTHORIZATION_H

#include <boost/noncopyable.hpp>
#include <map>
#include <string>
#include "llssdb/auth/user_info.h"

class IAuthorization : boost::noncopyable {
 public:
    virtual bool Registration(const std::string &login, const std::string &pass) = 0;

    virtual bool IsAuth(const std::string &login, const std::string &pass, int table_id) = 0;

    virtual bool RemoveUser(const std::string &login, const std::string &pass) = 0;

    virtual ~IAuthorization() = default;
};

#endif  // FAILLESS_IAUTHORIZATION_H
