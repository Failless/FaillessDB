#ifndef FAILLESS_LLSSDB_NETWORK_CONNECTION_H_
#define FAILLESS_LLSSDB_NETWORK_CONNECTION_H_

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace failless {
namespace db {
namespace network {

namespace ip = boost::asio::ip;
const size_t kMaxSize = 1024;

class Connection {
 public:
//    typedef boost::shared_ptr<Connection> pointer;

    Connection() = delete;
    explicit Connection(boost::asio::io_service& io_service);
    ~Connection() = default;

    void Read(const boost::system::error_code& err, size_t bytes_transferred);
    void Write(const boost::system::error_code& err, size_t bytes_transferred);
    void DoJob();
    ip::tcp::socket& GetSocket();
    Connection(Connection&) = delete;
    Connection(Connection&&) = delete;

 private:
    ip::tcp::socket socket_;
    //    std::vector<char> data_;
    std::string message = "Hello i can send you smth";
    char buffer_[kMaxSize]{};
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
