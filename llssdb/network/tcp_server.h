#ifndef LLSSDB_NETWORK_TCP_SERVER_H_
#define LLSSDB_NETWORK_TCP_SERVER_H_

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "llss3p/utils/queue.h"
#include "llssdb/network/tcp_server_interface.h"

namespace failless {
namespace db {
namespace network {

namespace ip = boost::asio::ip;

class TcpServer : public ITcpServer {
 public:
    TcpServer() = delete;
    TcpServer(common::utils::Queue<std::shared_ptr<Connection>>& queue, Host host);
    TcpServer(common::utils::Queue<std::shared_ptr<Connection>>& queue, const std::string& ip,
              unsigned short port);
    ~TcpServer() override = default;
    void SetConfig(std::string ip, int port) override;
    void Listen() override;
    Host GetSettings() override;
    void SetQueue(common::utils::Queue<std::shared_ptr<Connection>>& queue) override;

 protected:
    Host host_{};

 private:
    void Accept_();
    void AcceptHandler_(std::shared_ptr<Connection>& adaptor,
                        const boost::system::error_code& error);

    std::shared_ptr<common::utils::Queue<std::shared_ptr<Connection>>> queue_;

 protected:
    boost::asio::io_service io_service_;

 private:
    std::unique_ptr<ip::tcp::acceptor> acceptor_;
    std::vector<Connection> connections_;
    bool is_run_ = false;
};

}  // namespace network
}  // namespace db
}  // namespace failless

#endif  // LLSSDB_NETWORK_TCP_SERVER_H_