#ifndef TESTS_DB_TEST_SERVER_H_
#define TESTS_DB_TEST_SERVER_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <boost/chrono.hpp>
#include <boost/uuid/random_generator.hpp>
#include "llssdb/network/tcp_server.h"
#include "llssdb/network/tcp_server_interface.h"
#include "tests/tests_db/tcp_server_mock.h"
#include "tests/tests_db/test_tcp_server.h"

using ::testing::_;
using ::testing::AtLeast;

namespace failless {
namespace db {
namespace tests {

network::Response testFunction(network::Request &request) {
    return network::Response(request.GetData());
}

TEST(ServerTest, SetConnection) {
    auto server = std::shared_ptr<network::ITcpServer>(new network::TcpServer());
    int port = 11556;
    std::string ip = "127.0.0.1";
    server->SetConfig(ip, port);
    network::Request request{};
    std::function<network::Response(network::Request &)> foo =
        [&](network::Request &) { return testFunction(request); };
    server->SetResponseFunction(foo);
    auto host = server->GetSettings();
    EXPECT_EQ(host.ip, "127.0.0.1");
    EXPECT_EQ(host.port, 11556);
}

TEST(ServerTest, PushTask) {
    MockTcpServer mock_tcp_server{};
    std::string query("SET key1 ");
    boost::uuids::random_generator generator;
    boost::uuids::uuid client_id = generator();
    auto now = boost::chrono::system_clock::now();
    auto ms = boost::chrono::time_point_cast<boost::chrono::milliseconds>(now);
    common::Task task{0, nullptr, query, client_id,
                      boost::chrono::microseconds(ms.time_since_epoch().count())};
    EXPECT_CALL(mock_tcp_server, PushTask_(task)).Times(AtLeast(1));
}

TEST_F(TestTcpServerImpl, SendData) {
    std::string set_data("hello llsscli");
    network::Request request{};
    int8_t bin_data[3] = {1, 2, 3};
    request.SetData(bin_data, 3);
    std::function<network::Response(network::Request &)> func =
        [&](network::Request &) { return testFunction(request); };
    tcp_server->SetResponseFunction(func);
    auto data = test_client.Ping();
    EXPECT_EQ(data, set_data);
    //  tcp_server->Listen();
}

TEST(ServerTest, GetData) {}

TEST(ServerTest, Credentionals) {}

}  // namespace tests
}  // namespace db
}  // namespace failless

#endif  // TESTS_DB_TEST_SERVER_H_
