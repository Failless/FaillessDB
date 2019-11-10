#ifndef LLSSDB_NETWORK_TCP_SERVER_H_
#define LLSSDB_NETWORK_TCP_SERVER_H_

#include <boost/lockfree/queue.hpp>
#include "llssdb/engine/task.h"
#include "llssdb/network/tcp_server_interface.h"

namespace failless {
namespace db {
namespace network {

class TcpServer : public TcpServerInterface {
public:
  TcpServer() = default;
  explicit TcpServer(Host host);
  TcpServer(std::string ip, int port);
  ~TcpServer() override = default;
  void SetConfig(std::string ip, int port) override;
  void Listen() override;
  Host GetSettings() override;
  void SetResponseFunction(
      std::function<transfer::Response(transfer::Request &)> &generate_response)
      override;
protected:
  void PushTask_(engine::Task task);
  boost::lockfree::queue<engine::Task> *queue_ = nullptr;
  Host host_{};
};

} // namespace network
} // namespace db
} // namespace failless

#endif // LLSSDB_NETWORK_TCP_SERVER_H_
