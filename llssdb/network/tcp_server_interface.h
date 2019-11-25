#ifndef LLSSDB_NETWORK_TCP_SERVER_INTERFACE_H_
#define LLSSDB_NETWORK_TCP_SERVER_INTERFACE_H_

#include <boost/asio/ip/address.hpp>
#include <boost/noncopyable.hpp>
#include <functional>
#include <string>
#include "llssdb/network/transfer/response.h"

namespace failless {
namespace db {
namespace network {

struct Host {
    boost::asio::ip::address ip{};
    unsigned short port = 0;
    Host() = default;
    Host(std::string ip_, unsigned short port_)
        : ip(boost::asio::ip::address::from_string(ip_)), port(port_) {}
};

class ITcpServer : boost::noncopyable {
 public:
    virtual ~ITcpServer() = default;
    virtual void SetConfig(std::string ip, int port) = 0;
    virtual void Listen() = 0;
    virtual Host GetSettings() = 0;
    virtual void SetResponseFunction(std::function<Response(Request &)> &generate_response) = 0;

 protected:
    Host host_;
    std::function<Response(Request &)> generate_response_;
};

}  // namespace network
}  // namespace db
}  // namespace failless

#endif  // LLSSDB_NETWORK_TCP_SERVER_INTERFACE_H_
