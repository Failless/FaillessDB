#ifndef LLSSDB_NETWORK_TCP_SERVER_INTERFACE_H_
#define LLSSDB_NETWORK_TCP_SERVER_INTERFACE_H_

#include "network/transfer/response.h"
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
  virtual void SetConfig(std::string ip, int port) = 0;
  virtual void Listen() = 0;
  virtual Host GetSettings() = 0;
  virtual void
  SetResponseFunction(std::function<transfer::Response(transfer::Request &)>
                          &generate_response) = 0;
  virtual ~TcpServerInterface() = default;

protected:
  Host host;
  std::function<transfer::Response(transfer::Request &)> generate_response_;
};

} // namespace network
} // namespace llssdb
} // namespace failless

#endif // LLSSDB_NETWORK_TCP_SERVER_INTERFACE_H_
