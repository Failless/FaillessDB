#ifndef LLSSDB_NETWORK_TCP_SERVER_H_
#define LLSSDB_NETWORK_TCP_SERVER_H_

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/lockfree/queue.hpp>
#include "llss3p/utils/queue.h"
#include "llssdb/network/tcp_server_interface.h"
#include "llssdb/network/transfer/connection.h"
#include "llssdb/network/transfer/adapter.h"
#include "llssdb/utils/task.h"

namespace failless {
namespace db {
namespace network {

namespace ip = boost::asio::ip;

class TcpServer : public ITcpServer {
 public:
    TcpServer() = delete;
    TcpServer(common::utils::Queue<ConnectionAdapter>& queue, Host host);
    TcpServer(common::utils::Queue<ConnectionAdapter>& queue, std::string ip, unsigned short port);
    ~TcpServer() override = default;
    void SetConfig(std::string ip, int port) override;
    void Listen() override;
    Host GetSettings() override;
    void SetQueue(common::utils::Queue<ConnectionAdapter>& queue) override;

 protected:
    void PushTask_(utils::Task task);
    Host host_{};

 private:
    void Accept_();
    void AcceptHandler_(ConnectionAdapter adaptor, const boost::system::error_code& error);

    std::shared_ptr<common::utils::Queue<ConnectionAdapter>> queue_;
    boost::asio::io_service io_service_;
    std::unique_ptr<ip::tcp::acceptor> acceptor_;
    std::vector<utils::Task> tasks_;
    std::vector<Connection> connections_;
    bool is_run_ = false;
};

}  // namespace network
}  // namespace db
}  // namespace failless

#endif  // LLSSDB_NETWORK_TCP_SERVER_H_