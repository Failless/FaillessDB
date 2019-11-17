#ifndef LLSSDB_AUTH_REGISTRATION_H_
#define LLSSDB_AUTH_REGISTRATION_H_

#include <string>

class Registration {
 public:
    Registration() = default;
    bool Register(std::string login, std::string pass);  // WTF
    explicit Registration(std::string directory);
    ~Registration() = default;

 private:
    std::string server_db_conn_{};
    std::string login_{};
    std::string pass_{};
    std::string dir_{};
    bool CheckCollisions_();
    bool CreateDir_();
    bool UpdateServerDB_();
    // calling login function
};

#endif  // LLSSDB_AUTH_REGISTRATION_H_
