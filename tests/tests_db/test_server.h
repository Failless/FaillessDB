#ifndef TESTS_DB_TEST_SERVER_H_
#define TESTS_DB_TEST_SERVER_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <boost/chrono.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include "llssdb/network/tcp_server.h"
#include "llssdb/network/tcp_server_interface.h"
#include "tcp_server_mock.h"
#include "tests/tests_db/tcp_server_mock.h"
#include "tests/tests_db/test_tcp_server.h"

using ::testing::_;
using ::testing::AtLeast;

namespace failless {
namespace db {
namespace tests {


TEST(ServerTest, SetConnection) {
    int port = 11556;
    std::string ip = "127.0.0.1";
    failless::common::utils::Queue<std::shared_ptr<failless::db::network::Connection>> queue;
    auto server = std::shared_ptr<network::ITcpServer>(new network::TcpServer(queue, ip, port));
    auto host = server->GetSettings();
    EXPECT_EQ(boost::lexical_cast<std::string>(host.ip), "127.0.0.1");
    EXPECT_EQ(host.port, 11556);
}

TEST_F(TestTcpServerImpl, SendData) {
    std::string set_data("hello llsscli");
    uint8_t bin_data[3] = {1, 2, 3};
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
