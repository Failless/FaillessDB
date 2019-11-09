#ifndef LLSSDB_NETWORK_TCP_SERVER_H_
#define LLSSDB_NETWORK_TCP_SERVER_H_

#include <functional>
#include "llssdb/network/tcp_server_interface.h"


namespace failless {
namespace db {
namespace network {

class TcpServer : public TcpServerInterface {
public:
  TcpServer();
  ~TcpServer() = default;
  void setConfig(std::string ip, int port) override;
  void listen() override;
  Host getSettings() override;
  void setResponseFunction(
      std::function<Response(Request &)> &generate_response) override;

private:
  std::function<Response(Request &)> &generate_response_;
};

} // namespace network
} // namespace db
} // namespace failless

#endif // LLSSDB_NETWORK_TCP_SERVER_H_
