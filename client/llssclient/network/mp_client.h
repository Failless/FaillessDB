#ifndef LLSSCLIENT_NETWORK_MP_CLIENT_H_
#define LLSSCLIENT_NETWORK_MP_CLIENT_H_

//#include <msgpack.hpp>
#include "llssclient/network/mp_client_interface.h"

namespace failless {
namespace client {
namespace network {

using namespace boost::asio;

const int MAX_MSG = 1024;

class MpClient : public MpClientInterface {
public:
  MpClient() = default;
  ~MpClient() override = default;

  void Start(const ip::tcp::endpoint &ep) override;
  ptr Start(const ip::tcp::endpoint &ep, const std::string &username) override;
  void Stop() override;
  bool Started() override;

protected:
  size_t ReadComplete_(const boost::system::error_code &err,
                       size_t bytes) override;

private:
  char read_buffer_[MAX_MSG]{};
  char write_buffer_[MAX_MSG]{};
};

} // namespace network
} // namespace client
} // namespace failless

#endif // LLSSCLIENT_NETWORK_MP_CLIENT_H_
