#ifndef FAILLESS_CONFIG_CONFIG_H_
#define FAILLESS_CONFIG_CONFIG_H_

#include <string>
#include <vector>

namespace failless {
namespace client {
namespace config {

struct Data {
    std::unique_ptr< uint8_t[] > value;
    size_t size = 0;
    size_t folder_id = 0;
    std::unique_ptr< std::string > key;
};

struct Task {
    std::unique_ptr< std::string > client_id;
    std::unique_ptr< std::string > query;
    Data payload;
};

struct ClientConfig {
    std::string user_name = "";
    std::string user_request = "";

    int payload_dest_id = 0;
    std::string payload_key = "";

    std::string db_host = "";
    std::string db_port = "";

    ClientConfig(std::string user_n, std::string user_r, int payload_d, std::string payload_k, std::string db_h, std::string db_p) : user_name(user_n), user_request(user_r), payload_dest_id(payload_d), payload_key(payload_k), db_host(db_h), db_port(db_p) {}
};

struct NetworkConfig {
    std::string db_host = "";
    std::string db_port = "";

    NetworkConfig(std::string db_h, std::string db_p) : db_host(db_h), db_port(db_p) {}
};

} // namespace core
} // namespace llsscli
} // namespace failless

#endif //FAILLESS_CONFIG_CONFIG_H_
