#ifndef LLSSDB_TESTS_TEST_SERVER_WORKER_H
#define LLSSDB_TESTS_TEST_SERVER_WORKER_H


#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/engine/server_manager.h"

namespace failless::db::tests {

using ::testing::_;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;

using engine::ServerManager;

class MockServerManager : public ServerManager {
public:
//    MockServerManager() { Instance(); };
    MOCK_METHOD0(Instance, void());
};

TEST(NodeTest, TestCall) {
//    MockServerManager mockServerManager;
//    EXPECT_CALL(mockNode, TestCall()).Times(AtLeast(1));
//    EXPECT_TRUE(engine::ServerManager::Instance()->TestTest());
}

} // namespace failless::llssdb::tests_cli


#endif // LLSSDB_TESTS_TEST_SERVER_WORKER_H
