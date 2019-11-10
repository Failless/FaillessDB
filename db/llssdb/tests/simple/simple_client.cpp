#include "llssdb/tests/simple/simple_client.h"

#include <utility>

namespace failless {
namespace db {
namespace tests {

std::string SimpleClient::Ping() { return std::string("pong"); }

void SimpleClient::SetConfig(network::Host host) {
  host_ = std::move(host);
}

void SimpleClient::SetConfig(std::string ip, int port) {
  host_ = network::Host(std::move(ip), port);
}

} // namespace tests
} // namespace db
} // namespace failless
