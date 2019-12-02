#ifndef LLSSDB_AUTH_AUTHORIZATION_H_
#define LLSSDB_AUTH_AUTHORIZATION_H_

#include <string>
#include "llssdb/auth/iauthorization.h"
#include "llssdb/auth/user_info.h"

bool SimpleSHA256(const std::string &input, unsigned char *md);

class Authorization : public IAuthorization {
 public:
    Authorization() = default;

    explicit Authorization(std::string login);

    bool Registration(const std::string &login, const std::string &pass) override;

    bool RemoveUser(const std::string &login, const std::string &pass) override;

    bool IsAuth(const std::string &login, const std::string &pass, int table_id) override;

    ~Authorization() override = default;

 private:
    std::map<std::string, UserInfo> Users_;

    bool CheckCollisions_(const std::string &login);

    void Hasher_(const std::string &login, std::string pass, unsigned char *md);
};

#endif  // LLSSDB_AUTH_AUTHORIZATION_H_
