#ifndef FAILLESS_CONFIG_CONFIG_H_
#define FAILLESS_CONFIG_CONFIG_H_

#include <string>
#include <vector>

namespace failless {
namespace client {
namespace config {

using std::string;
using std::unique_ptr;

struct Data {
    unique_ptr< uint8_t[] > value;
    size_t size = 0;
    size_t folder_id = 0;
    unique_ptr< string > key;
};

struct Task {
    unique_ptr< string > client_id;
    unique_ptr< string > query;
    Data payload;
};

struct ClientConfig {
    string user_name = "";
    string user_request = "";

    int payload_dest_id = 0;
    string payload_key = "";

    string db_host = "";
    string db_port = "";

    ClientConfig(string user_n, string user_r, int payload_d, string payload_k, string db_h, string db_p) : user_name(user_n), user_request(user_r), payload_dest_id(payload_d), payload_key(payload_k), db_host(db_h), db_port(db_p) {}
};

struct NetworkConfig {
    string db_host = "";
    string db_port = "";

    NetworkConfig(string db_h, string db_p) : db_host(db_h), db_port(db_p) {}
};

} // namespace core
} // namespace llsscli
} // namespace failless

#endif //FAILLESS_CONFIG_CONFIG_H_
