#ifndef LLSSDB_NETWORK_TCP_SERVER_H_
#define LLSSDB_NETWORK_TCP_SERVER_H_

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/lockfree/queue.hpp>
#include "llss3p/utils/queue.h"
#include "llssdb/network/connection.h"
#include "llssdb/network/tcp_server_interface.h"
#include "llssdb/utils/task.h"

namespace failless {
namespace db {
namespace network {

namespace ip = boost::asio::ip;

class TcpServer : public ITcpServer {
 public:
    TcpServer() = delete;
    explicit TcpServer(Host host);
    TcpServer(std::string ip, unsigned short port);
    ~TcpServer() override = default;
    void SetConfig(std::string ip, int port) override;
    void Listen() override;
    Host GetSettings() override;
    void SetResponseFunction(std::function<Response(Request &)> &generate_response) override;

 protected:
    void PushTask_(utils::Task task);
    Host host_{};

 private:
    void Accept_();
    void AcceptHandler_(ConnectionAdapter adaptor, const boost::system::error_code &error);

    common::utils::Queue<Connection> *queue_ = nullptr;
    boost::asio::io_service io_service_;
    //    ip::tcp::socket *socket_;
    /// The acceptor object used to accept incoming socket connections.
    std::unique_ptr<ip::tcp::acceptor> acceptor_;
    /// The data to be sent to each client.
    std::vector<utils::Task> tasks_;
    std::vector<Connection> connections_;
    bool is_run_ = false;
};

}  // namespace network
}  // namespace db
}  // namespace failless

#endif  // LLSSDB_NETWORK_TCP_SERVER_H_