#ifndef FAILLESS_CONFIG_H
#define FAILLESS_CONFIG_H

#include <string>
#include <vector>

namespace failless {
namespace client {
namespace config {

using std::string;
using std::vector;

struct Data {
    std::vector<uint8_t> value;
    size_t size = 0;
    int folder_id = 0;
    string key = "";
};

struct Task {
    string client_id = "";
    string query = "";
    Data payload;
};

struct ClientConfig {
    string user_name = "";
    string user_request = "";

    int payload_dest_id = 0;
    string payload_key = "";

    string db_host = "";
    string db_port = "";
};

struct NetworkConfig {
    string db_host = "";
    string db_port = "";
};

} // namespace core
} // namespace llsscli
} // namespace failless

#endif //FAILLESS_CONFIG_H
