//
// Created by sergey on 09.11.2019.
//

#ifndef FAILLESS_REGISTRATION_H
#define FAILLESS_REGISTRATION_H

#include <string>

class Registration {
public:
    bool register(std::string login, std::string pass);  // WTF
    explicit Registration(std::string directory);
    ~Registration();
private:
    std::string server_db_conn_;
    std::string login_;
    std::string pass_;
    std::string dir_;
    bool check_collisions();
    bool create_dir();
    bool update_server_db();
    // calling login function
};

#endif //FAILLESS_REGISTRATION_H
