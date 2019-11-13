#ifndef FAILLESS_NETWORK_MP_CLIENT_INTERFACE_H_
#define FAILLESS_NETWORK_MP_CLIENT_INTERFACE_H_

#include <boost/asio/ip/tcp.hpp>
#include <boost/core/noncopyable.hpp>
#include <string>

using namespace boost::asio;

namespace failless {
namespace client {
namespace network {

class MpClientInterface : boost::noncopyable {
public:
  MpClientInterface() = default;
  virtual ~MpClientInterface() = default;
  typedef boost::system::error_code error_code;
  typedef boost::shared_ptr<MpClientInterface> ptr;

  virtual void Start(const ip::tcp::endpoint &ep) = 0;
  virtual ptr Start(const ip::tcp::endpoint &ep,
                    const std::string &username) = 0;
  virtual void Stop() = 0;
  virtual bool Started() = 0;

protected:
  virtual size_t ReadComplete_(const boost::system::error_code &err,
                               size_t bytes) = 0;
};

} // namespace network
} // namespace llsscli
} // namespace failless

#endif // FAILLESS_NETWORK_MP_CLIENT_INTERFACE_H_
