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

struct Data {
    // https://github.com/msgpack/msgpack-c/issues/243
    std::unique_ptr<std::vector<unsigned char>> value;
    size_t size = 0;
    size_t folder_id = 0;
    std::unique_ptr<std::string> key;

    // http://ludwig125.hatenablog.com/entry/2017/12/01/232952
    // http://ludwig125.hatenablog.com/entry/2017/11/22/234335
    MSGPACK_DEFINE_MAP(value, size, folder_id, key);

    Data() = default;
    Data(std::unique_ptr<std::vector<unsigned char>>& v, size_t s, size_t f_id,
         std::unique_ptr<std::string>& k)
        : value(std::move(v)), size(s), folder_id(f_id), key(std::move(k)) {}
};

struct Task {
    std::unique_ptr<std::string> client_id;
    std::unique_ptr<std::string> query;
    std::unique_ptr<Data> payload;

    MSGPACK_DEFINE_MAP(client_id, query, payload);

    Task() = default;
    Task(std::unique_ptr<std::string>& c_id, std::unique_ptr<std::string>& q,
         std::unique_ptr<Data>& p)
        : client_id(std::move(c_id)), query(std::move(q)), payload(std::move(p)) {}
};

struct ClientConfig {
    std::string user_name{};
    std::string user_request{};

    int payload_dest_id = 0;
    std::string payload_key{};

    std::string db_host{};
    std::string db_port{};

    size_t client_num = 0;

    ClientConfig() = default;

    ClientConfig(std::string user_n, std::string user_r, int payload_d, std::string payload_k,
                 std::string db_h, std::string db_p, size_t c_num)
        : user_name(std::move(user_n)),
          user_request(std::move(user_r)),
          payload_dest_id(payload_d),
          payload_key(std::move(payload_k)),
          db_host(std::move(db_h)),
          db_port(std::move(db_p)),
          client_num(std::move(c_num)) {}
};

struct NetworkConfig {
    std::string db_host{};
    std::string db_port{};
    size_t client_num = 0;

    NetworkConfig() = default;

    NetworkConfig(std::string db_h, std::string db_p, size_t c_num)
        : db_host(std::move(db_h)), db_port(std::move(db_p)), client_num(std::move(c_num)) {}
};

struct NetworkConnectTask {
    std::shared_ptr<tcp::socket> socket;
    std::shared_ptr<boost::asio::io_service> io_service;
    std::shared_ptr<std::stringstream> client_task;
    std::shared_ptr<std::function<size_t()>> client_callback;

    NetworkConnectTask() = default;

    NetworkConnectTask(std::shared_ptr<tcp::socket>& sock,
                       std::shared_ptr<boost::asio::io_service>& io_serv,
                       std::shared_ptr<std::stringstream>& cl_task,
                       std::shared_ptr<std::function<size_t()>>& callback)
        : socket(sock), io_service(io_serv), client_task(cl_task), client_callback(callback) {}
};

}  // namespace config
}  // namespace client
}  // namespace failless

#endif  // FAILLESS_CONFIG_CONFIG_H_
