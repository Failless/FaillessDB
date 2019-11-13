#ifndef LLSSDB_SIMPLE_CLIENT_H
#define LLSSDB_SIMPLE_CLIENT_H

#include <llssdb/network/tcp_server_interface.h>
#include <boost/noncopyable.hpp>
#include <string>

namespace failless {
namespace db {
namespace tests {
class SimpleClient : boost::noncopyable {
public:
  SimpleClient() = default;
  ~SimpleClient() = default;
  std::string Ping();
  void SetConfig(network::Host);
  void SetConfig(std::string ip, int port);

private:
  network::Host host_;
};

} // namespace tests_cli
} // namespace llssdb
} // namespace failless

#endif // LLSSDB_SIMPLE_CLIENT_H
