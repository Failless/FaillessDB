#ifndef TESTS_DB_SIMPLE_CLIENT_H_
#define TESTS_DB_SIMPLE_CLIENT_H_

#include <boost/noncopyable.hpp>
#include <string>
#include "llssdb/network/tcp_server_interface.h"

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

}  // namespace tests
}  // namespace db
}  // namespace failless

#endif  // TESTS_DB_SIMPLE_CLIENT_H_
