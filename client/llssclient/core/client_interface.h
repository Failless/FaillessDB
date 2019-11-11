#ifndef LLSSCLIENT_CORE_CLIENT_INTERFACE_H_
#define LLSSCLIENT_CORE_CLIENT_INTERFACE_H_

#include <boost/noncopyable.hpp>
#include <string>

namespace failless {
namespace client {
namespace core {

class ClientInterface : boost::noncopyable {
public:
  ClientInterface() = default;
  ~ClientInterface() = default;
  virtual size_t ReadQuery(std::string query) = 0;
  virtual size_t Send() = 0;
  virtual size_t Params(int argc, char **argv) = 0;
  int Test(std::string query) {
    ReadQuery(std::move(query));
    return EXIT_SUCCESS;
  }

private:
  virtual size_t ParseQuery_(std::string query) = 0;
};

} // namespace core
} // namespace client
} // namespace failless

#endif // LLSSCLIENT_CORE_CLIENT_INTERFACE_H_
