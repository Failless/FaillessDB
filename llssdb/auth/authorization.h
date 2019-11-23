#ifndef LLSSDB_AUTH_AUTHORIZATION_H_
#define LLSSDB_AUTH_AUTHORIZATION_H_

#include "llssdb/auth/iauthorization.h"
// #include "llssdb/auth/user_info.h"

#include <algorithm>
#include <string>

#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>

bool simpleSHA256(void* input, unsigned long length, unsigned char* md);

class Authorization : public IAuthorization {
public:
    Authorization() = default;
    explicit Authorization(std::string login);
    bool Registration(std::string login, std::string pass) override;
    bool RemoveUser(std::string login, std::string pass) override;
    bool IsAuth(std::string login, std::string pass, int table_id) override;
    ~Authorization() override = default;

private:
    std::map<std::string, UserInfo> Users_;
    virtual bool CheckCollisions_(std::string login);
    virtual unsigned char *Hasher_(std::string login, std::string pass);
};
#endif // LLSSDB_AUTH_AUTHORIZATION_H_
