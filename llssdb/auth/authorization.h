#ifndef LLSSDB_AUTH_AUTHORIZATION_H_
#define LLSSDB_AUTH_AUTHORIZATION_H_

#include "llssdb/auth/iauthorization.h"

class Authorization : public IAuthorization {
public:
    Authorization() = default;
    explicit Authorization(std::string directory);
    bool Registration(std::string login, std::string pass) override;
    bool SignIn(std::string login, std::string pass) override;
    ~Authorization() = default;

private:
    std::map<std::string, struct UserInfo> Users_;
    static std::string Hasher_(std::string login);
    bool IsAuth_(std::string login);
    virtual bool CheckCollisions_(std::string login);
// add tire_
};
#endif // LLSSDB_AUTH_AUTHORIZATION_H_
