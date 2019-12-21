#ifndef FAILLESS_CONFIG_CONFIG_H_
#define FAILLESS_CONFIG_CONFIG_H_

#include <boost/asio.hpp>
#include <functional>
#include <msgpack.hpp>
#include <string>
#include <utility>

namespace failless {
namespace client {
namespace config {

using boost::asio::ip::tcp;

struct ClientConfig {
    std::string user_name{};
    std::string user_pass{};
    std::string user_request{};

    int payload_dest_id = 0;
    std::string payload_key{};

    std::string db_host{};
    std::string db_port{};

    ClientConfig() = default;

    ClientConfig(std::string user_n, std::string user_p, std::string user_r, int payload_d, std::string payload_k,
                 std::string db_h, std::string db_p)
        : user_name(std::move(user_n)),
          user_pass(std::move(user_p)),
          user_request(std::move(user_r)),
          payload_dest_id(payload_d),
          payload_key(std::move(payload_k)),
          db_host(std::move(db_h)),
          db_port(std::move(db_p)) {}
};

struct NetworkConfig {
    std::string db_host{};
    std::string db_port{};

    NetworkConfig() = default;

    NetworkConfig(std::string db_h, std::string db_p)
        : db_host(std::move(db_h)), db_port(std::move(db_p)) {}
};

struct NetworkConnectTask {
    std::shared_ptr<tcp::socket> socket;
    std::shared_ptr<boost::asio::io_service> io_service;
    std::shared_ptr<std::stringstream> client_task;
    std::shared_ptr<std::function<size_t(char*, size_t)>> client_callback;

    NetworkConnectTask() = default;

    NetworkConnectTask(std::shared_ptr<tcp::socket>& sock,
                       std::shared_ptr<boost::asio::io_service>& io_serv,
                       std::shared_ptr<std::stringstream>& cl_task,
                       std::shared_ptr<std::function<size_t(char*, size_t)>>& callback)
        : socket(sock), io_service(io_serv), client_task(cl_task), client_callback(callback) {}
};

}  // namespace config
}  // namespace client
}  // namespace failless

#endif  // FAILLESS_CONFIG_CONFIG_H_
