#ifndef LLSSDB_NETWORK_TCP_SERVER_INTERFACE_H_
#define LLSSDB_NETWORK_TCP_SERVER_INTERFACE_H_

#include "llssdb/network/transfer/response.h"
#include <boost/noncopyable.hpp>
#include <functional>
#include <string>

namespace failless {
namespace db {
namespace network {

struct Host {
  std::string ip{};
  int port{};
  Host() = default;
  Host(std::string ip_, int port_) : ip(std::move(ip_)), port(port_) {}
};

class TcpServerInterface : boost::noncopyable {
public:
  TcpServerInterface() = default;
  virtual void setConfig(std::string ip, int port) = 0;
  virtual void listen() = 0;
  virtual Host getSettings() = 0;
  virtual void
  setResponseFunction(std::function<transfer::Response(transfer::Request &)>
                          &generate_response) = 0;

protected:
  Host host;
  std::function<transfer::Response(transfer::Request &)> generate_response_;
};

} // namespace network
} // namespace db
} // namespace failless

#endif // LLSSDB_NETWORK_TCP_SERVER_INTERFACE_H_
