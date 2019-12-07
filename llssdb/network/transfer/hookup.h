#ifndef FAILLESS_LLSSDB_NETWORK_TRANSFER_HOOKUP_H_
#define FAILLESS_LLSSDB_NETWORK_TRANSFER_HOOKUP_H_

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "llss3p/utils/packet.h"
#include "llssdb/utils/task.h"

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
//    Connection(Connection&) = delete;
    Connection(Connection&&) = delete;

    void Read(const boost::system::error_code& err, size_t bytes_transferred);
    void Write(const boost::system::error_code& err, size_t bytes_transferred);
    void DoJob();
    void SendData(common::utils::Packet& data);
    bool GetMetaData(utils::ServerTask& task);
    common::utils::Packet* GetPacket();
    ip::tcp::socket& GetSocket();
    [[nodiscard]] bool HasData() const;

 private:
    ip::tcp::socket socket_;
    char buffer_[kMaxSize]{};
    bool has_ = false;
    common::utils::Packet packet_;
};

}  // namespace network
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_NETWORK_TRANSFER_HOOKUP_H_
