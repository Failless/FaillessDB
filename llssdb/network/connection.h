#ifndef FAILLESS_LLSSDB_NETWORK_CONNECTION_H_
#define FAILLESS_LLSSDB_NETWORK_CONNECTION_H_

#include <llss3p/utils/packet.h>
#include <llssdb/utils/task.h>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace failless {
namespace db {
namespace network {

namespace ip = boost::asio::ip;
const size_t kMaxSize = 1024;

class Connection {
 public:
    Connection() = delete;
    explicit Connection(boost::asio::io_service& io_service);
    ~Connection() = default;
    Connection(Connection&) = delete;
    Connection(Connection&&) = delete;

    void Read(const boost::system::error_code& err, size_t bytes_transferred);
    void Write(const boost::system::error_code& err, size_t bytes_transferred);
    void DoJob();
    void SendData(common::utils::Packet data);
    bool GetData(utils::Task& task);
    ip::tcp::socket& GetSocket();
    [[nodiscard]] bool HasData() const;

 private:
    ip::tcp::socket socket_;
    //    std::vector<char> data_;
    std::string message = "Hello i can send you smth";
    char buffer_[kMaxSize]{};
    bool has_= false;
    common::utils::Packet packet_;
};

struct ConnectionAdapter {
    ConnectionAdapter() : conn(nullptr) {}
    explicit ConnectionAdapter(boost::asio::io_service &io_service) {
        conn.reset(new Connection(io_service));
    }
    ~ConnectionAdapter() = default;
    std::shared_ptr<Connection> conn;
};

}  // namespace network
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_NETWORK_CONNECTION_H_
