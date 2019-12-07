
#ifndef TESTS_DB_TCP_SERVER_MOCK_H_
#define TESTS_DB_TCP_SERVER_MOCK_H_

#include <gmock/gmock.h>
#include <utility>
#include "llssdb/network/tcp_server.h"

namespace failless {
namespace db {
namespace tests {
/*
class MockTcpServer : public network::TcpServer {
 public:
    MockTcpServer(common::utils::Queue<network::Connection>& queue, const std::string& ip,
                  unsigned short port)
        : network::TcpServer(queue, ip, port) {}
    MOCK_METHOD1(PushTask_, void(utils::Task task));
};
*/
}  // namespace tests
}  // namespace db
}  // namespace failless

#endif  // TESTS_DB_TCP_SERVER_MOCK_H_