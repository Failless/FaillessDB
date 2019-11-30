#ifndef TESTS_DB_TEST_TCP_SERVER_H_
#define TESTS_DB_TEST_TCP_SERVER_H_

#include <gtest/gtest.h>
#include "llssdb/network/tcp_server.h"
#include "tests/tests_db/simple/simple_client.h"

namespace failless {
namespace db {
namespace tests {

class TestTcpServerImpl : public ::testing::Test {
 public:
    void SetUp() {
        network::Host host("127.0.0.1", 11556);
        tcp_server.reset(new network::TcpServer(host));
        test_client.SetConfig(host);
    }

    std::shared_ptr<network::TcpServer> tcp_server{};
    SimpleClient test_client{};
};

}  // namespace tests
}  // namespace db
}  // namespace failless

#endif  // TESTS_DB_TEST_TCP_SERVER_H_