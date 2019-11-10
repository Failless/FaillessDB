//
// Created by sergey on 09.11.2019.
//

#ifndef FAILLESS_DISCONNECTOR_H
#define FAILLESS_DISCONNECTOR_H

#include <string>

class Disconnector {
public:
    bool Disconnect(std::string token);  // (token)
    explicit Disconnector(std::string directory);
    ~Disconnector() = default;
private:
    std::string server_db_conn_;
    std::string pid_;
    std::string dir_;
    std::string token_;
    bool Send_msg_to_SM();  // how?
};

#endif //FAILLESS_DISCONNECTOR_H
