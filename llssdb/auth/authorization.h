#ifndef LLSSDB_AUTH_AUTHORIZATION_H_
#define LLSSDB_AUTH_AUTHORIZATION_H_

#include <string>
#include "llssdb/auth/iauthorization.h"
#include "llssdb/auth/user_info.h"

bool SimpleSHA256(unsigned char *input, unsigned long length, unsigned char *md);

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

    unsigned char *Hasher_(const std::string &login, std::string pass, unsigned char *pass_hash);
};

#endif  // LLSSDB_AUTH_AUTHORIZATION_H_
