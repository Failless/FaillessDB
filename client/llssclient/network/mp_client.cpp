#include "llssclient/network/mp_client.h"

namespace failless {
namespace client {
namespace network {

void MpClient::Stop() {}

size_t MpClient::ReadComplete_(const boost::system::error_code &err,
                               size_t bytes) {
  return 0;
}

bool MpClient::Started() { return false; }

void MpClient::Start(const boost::asio::ip::tcp::endpoint &ep) {}

MpClient::ptr MpClient::Start(const boost::asio::ip::tcp::endpoint &ep,
                              const std::string &username) {
  return MpClient::ptr();
}

} // namespace network
} // namespace client
} // namespace failless
