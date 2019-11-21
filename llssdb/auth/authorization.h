#ifndef LLSSDB_AUTH_AUTHORIZATION_H_
#define LLSSDB_AUTH_AUTHORIZATION_H_

#include "llssdb/auth/iauthorization.h"
// #include "llssdb/auth/user_info.h"

#include <algorithm>
#include <string>

#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>


class Authorization : public IAuthorization {
public:
    Authorization() = default;
    explicit Authorization(std::string login);
    bool Registration(std::string login, std::string pass) override;
    bool SignIn(std::string login, std::string pass) override;
    ~Authorization() = default;

private:
    std::map<std::string, UserInfo> Users_;
    static std::string toString_(const boost::uuids::detail::md5::digest_type &digest);
    virtual bool IsAuth_(std::string login);
    virtual bool CheckCollisions_(std::string login);
    virtual std::string Hasher_(std::string login, std::string pass);
};
#endif // LLSSDB_AUTH_AUTHORIZATION_H_
