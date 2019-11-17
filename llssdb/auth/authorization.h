#ifndef LLSSDB_AUTH_AUTHORIZATION_H_
#define LLSSDB_AUTH_AUTHORIZATION_H_

#include <string>
#include "llssdb/auth/registration.h"
#include "llssdb/auth/signin.h"

class IAuthorization {
 public:
    virtual bool Reg(std::string login, std::string pass) = 0;
    virtual bool Login(std::string login, std::string pass) = 0;
    explicit IAuthorization() = default;
    ~IAuthorization() = default;

    int Test(std::string login, std::string pass) {
        Reg(std::move(login), std::move(pass));
        return EXIT_SUCCESS;
    }
};

class Authorization : public IAuthorization {
 public:
    Authorization() = default;
    explicit Authorization(std::string directory);
    ~Authorization() = default;

    bool Reg(std::string login, std::string pass) override;
    bool Login(std::string login, std::string pass) override;

 private:
    std::string server_db_conn_{};
    // Registration reg_;
    // SignIn sign_;
};
#endif  // LLSSDB_AUTH_AUTHORIZATION_H_
