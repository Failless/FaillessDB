//
// Created by sergey on 09.11.2019.
//

#ifndef FAILLESS_DISCONNECTOR_H
#define FAILLESS_DISCONNECTOR_H

#include <string>

class Disconnector {
public:
    bool disconnect(std::string token);  // (token)
    explicit Disconnector(std::string directory);
    ~Disconnector();
private:
    std::string server_db_conn_;
    std::string pid_;
    std::string dir_;
    std::string token_;
    bool send_msg_to_SM();  // how?
    bool free_RAM();
    bool copy_to_HDD();
    // calling login function
};

#endif //FAILLESS_DISCONNECTOR_H
