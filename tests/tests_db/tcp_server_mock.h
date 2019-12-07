#ifndef TESTS_DB_TCP_SERVER_MOCK_H_
#define TESTS_DB_TCP_SERVER_MOCK_H_

#include <gmock/gmock.h>
#include <utility>
#include "llssdb/network/tcp_server.h"

namespace failless {
namespace db {
namespace tests {
class MockConnection : public network::Connection {
 public:
    explicit MockConnection(boost::asio::io_service& io_service) : Connection(io_service) {}
    MOCK_METHOD0(DoJob, void());
};

class MockQueue : public decltype(common::utils::Queue<std::shared_ptr<network::Connection>>()) {
 public:
    MockQueue() : Queue() {}
    MOCK_METHOD1(Push, void(const MockConnection&));
};

class MockTcpServer : public network::TcpServer {
 public:
    MockTcpServer(common::utils::Queue<std::shared_ptr<network::Connection>>& queue,
                  const std::string& ip, unsigned short port)
        : network::TcpServer(queue, ip, port) {}

    void FakeCall() {
        Accept_();
    }
    boost::asio::io_service& GetIO() {
        return io_service_;
    }
    MOCK_METHOD0(Accept_, void());
};

TEST(Server, Intergration) {
    MockQueue queue;
    // it's a temporary action to avoid double free or corruption (out)
    auto* server = new MockTcpServer(queue, "127.0.0.1", 11556);
    MockConnection conn(server->GetIO());
    EXPECT_CALL(*server, Accept_()).Times(1);
    EXPECT_CALL(queue, Push).Times(1);
//    EXPECT_CALL(conn, DoJob).Times(1);
    server->FakeCall();
}

}  // namespace tests
}  // namespace db
}  // namespace failless

#endif  // TESTS_DB_TCP_SERVER_MOCK_H_