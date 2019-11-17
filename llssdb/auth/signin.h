#ifndef LLSSDB_AUTH_SIGNIN_H_
#define LLSSDB_AUTH_SIGNIN_H_

#include <string>

class SignIn {
 public:
    SignIn() = default;
    explicit SignIn(std::string directory);
    std::string Login(std::string login, std::string pass);  // token
    ~SignIn() = default;

 private:
    bool SendMsgToSM_();  // how?

    std::string server_db_conn_{};
    std::string login_{};
    std::string pass_{};
    std::string dir_{};
    std::string token_{};
    // calling login function
};

#endif  // LLSSDB_AUTH_SIGNIN_H_
