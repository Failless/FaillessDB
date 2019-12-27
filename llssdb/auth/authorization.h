#ifndef LLSSDB_AUTH_AUTHORIZATION_H_
#define LLSSDB_AUTH_AUTHORIZATION_H_

#include <string>
#include "llssdb/auth/iauthorization.h"
#include "llssdb/auth/user_info.h"


namespace failless {
namespace db {
namespace auth {


class Authorization : public IAuthorization {
 public:
    explicit Authorization();

    explicit Authorization(std::string login);

    bool Registration(const std::string &login, const std::string &pass, int folder_id) override;

    bool RemoveUser(const std::string &login, const std::string &pass) override;

    bool IsAuth(const std::string &login, const std::string &pass, int table_id) override;

    ~Authorization() override = default;

 private:
    std::map<std::string, UserInfo> Users_;

    bool CheckCollisions_(const std::string &login);

    void Hasher_(const std::string &login, std::string pass, unsigned char *md);

    static bool AddingUser_(const UserInfo &User);
};

}  // namespace auth
}  // namespace db
}  // namespace failless

#endif  // LLSSDB_AUTH_AUTHORIZATION_H_
