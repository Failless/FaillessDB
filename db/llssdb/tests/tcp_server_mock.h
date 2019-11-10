#ifndef LLSSDB_TESTS_TCP_SERVER_MOCK_H_
#define LLSSDB_TESTS_TCP_SERVER_MOCK_H_

#include "llssdb/network/tcp_server.h"
#include <gmock/gmock.h>

namespace failless {
namespace db {
namespace tests {

class MockTcpServer : public network::TcpServer {
public:
  MOCK_METHOD1(PushTask_, void(engine::Task task));
};

} // namespace tests
} // namespace db
} // namespace failless

#endif // LLSSDB_TESTS_TCP_SERVER_MOCK_H_
