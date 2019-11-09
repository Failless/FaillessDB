#ifndef LLSSDB_NETWORK_TCP_SERVER_INTERFACE_H_
#define LLSSDB_NETWORK_TCP_SERVER_INTERFACE_H_

#include <boost/noncopyable.hpp>
#include <string>

struct Host {
  std::string ip;
  int port;
  Host(std::string ip_, int port_) : ip(std::move(ip_)), port(port_) {}
};

class TcpServerInterface : boost::noncopyable {
public:
  TcpServerInterface() = default;
  virtual void setConfig(std::string ip, int port) = 0;
  virtual void listen() = 0;
  virtual Host getSettings() = 0;
  virtual void setResponseFunction(
      std::function<Response(Request &)> &generate_response) = 0;

protected:
  Host host;
};

#endif // LLSSDB_NETWORK_TCP_SERVER_INTERFACE_H_
