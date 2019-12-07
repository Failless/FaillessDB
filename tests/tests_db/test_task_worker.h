#ifndef TESTS_TESTS_DB_TEST_TASK_WORKER_H_
#define TESTS_TESTS_DB_TEST_TASK_WORKER_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

#include "llssdb/utils/task.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/task_worker.h"
#include "tests/tests_db/mocks.h"
#include "tests/tests_db/test_file_system.h"

namespace failless::db::tests {

std::shared_ptr<network::TestConnection> create_test_connection(common::enums::operators command) {
    boost::asio::io_service ios;
    common::utils::Packet packet;
    packet.command = command;
    size_t size = 3;
    std::vector<uint8_t> value = {1, 2, 3};
    auto key = new std::string("test_key");
    packet.data = {0, size, value, key};
    return std::make_shared<network::TestConnection>(ios, packet);
}

TEST(TaskManager, Get_and_Set) {
    common::utils::Queue<std::shared_ptr<network::Connection>> q;
    folder::TaskWorker tw(q, kTestDbPath);

    auto conn = create_test_connection(common::enums::operators::SET);

    int result1 = tw.DoTask(conn);

    conn->GetPacket()->command = common::enums::operators::GET;
    int result2 = tw.DoTask(conn);

    EXPECT_EQ(result1, result2);

    // clear
    conn->GetPacket()->command = common::enums::operators::DELETE;
    tw.DoTask(conn);
}

TEST(TaskManager, Calling_Self_Set) {
    common::utils::Queue<std::shared_ptr<network::Connection>> q;
    MockTaskWorker mockTaskWorker(q, kTestDbPath);

    auto conn = create_test_connection(common::enums::operators::SET);

    EXPECT_CALL(mockTaskWorker, Set(conn->GetPacket()->data)).Times(1);
    EXPECT_EQ(mockTaskWorker.DoTask(conn), EXIT_SUCCESS);
}


TEST(TaskManager, Calling_Self_Read) {
    common::utils::Queue<std::shared_ptr<network::Connection>> q;
    MockTaskWorker mockTaskWorker(q, kTestDbPath);

    auto conn = create_test_connection(common::enums::operators::GET);

    EXPECT_CALL(mockTaskWorker, Read(conn->GetPacket()->data)).Times(1);
    EXPECT_EQ(mockTaskWorker.DoTask(conn), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Self_Update) {
    common::utils::Queue<std::shared_ptr<network::Connection>> q;
    MockTaskWorker mockTaskWorker(q, kTestDbPath);

    auto conn = create_test_connection(common::enums::operators::UPDATE);

    EXPECT_CALL(mockTaskWorker, Update(conn->GetPacket()->data)).Times(1);
    EXPECT_EQ(mockTaskWorker.DoTask(conn), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Self_Delete) {
    common::utils::Queue<std::shared_ptr<network::Connection>> q;
    MockTaskWorker mockTaskWorker(q, kTestDbPath);

    auto conn = create_test_connection(common::enums::operators::DELETE);

    EXPECT_CALL(mockTaskWorker, Delete(conn->GetPacket()->data)).Times(1);
    EXPECT_EQ(mockTaskWorker.DoTask(conn), EXIT_SUCCESS);
}

} // namespace failless::db::tests

#endif // TESTS_TESTS_DB_TEST_TASK_WORKER_H
