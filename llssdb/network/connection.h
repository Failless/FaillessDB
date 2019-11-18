#ifndef FAILLESS_LLSSDB_NETWORK_CONNECTION_H_
#define FAILLESS_LLSSDB_NETWORK_CONNECTION_H_

#include <boost/noncopyable.hpp>
#include <boost/asio/ip/tcp.hpp>

namespace failless {
namespace db {
namespace network {

namespace ip = boost::asio::ip;
const size_t kMaxSize = 1024;

class Connection : boost::noncopyable {
 public:
    Connection() = delete;
    explicit Connection(ip::tcp::socket);
    ~Connection() = default;

    void Read();
    void Write(size_t size);

 private:
    ip::tcp::socket socket_;
    std::unique_ptr<std::vector<std::byte>> data_;
};

}
}
}

#endif  // FAILLESS_LLSSDB_NETWORK_CONNECTION_H_
