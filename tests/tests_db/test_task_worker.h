#ifndef FAILLESS_TESTS_TESTS_DB_TEST_TASK_WORKER_H_
#define FAILLESS_TESTS_TESTS_DB_TEST_TASK_WORKER_H_


#include "llssdb/folder/task_worker.h"

#include <cstdlib>
#include <memory>

#include <boost/asio.hpp>
#include <gtest/gtest.h>

#include "llss3p/enums/operators.h"
#include "llssdb/network/transfer/hookup.h"
#include "tests/tests_db/mocks.h"

namespace failless::db::tests {

std::shared_ptr<network::TestConnection> create_test_connection(common::enums::operators command) {
    boost::asio::io_service ios;
    common::utils::Packet packet;
    size_t size = 3;
    std::vector<uint8_t> value = {1, 2, 3};
    std::string key = "test_key";
    packet.data = {1, size, value, &key};
    packet.login = "test_user";
    packet.command = command;
    return std::make_shared<network::TestConnection>(ios, packet);
}

TEST(TaskManager, Get_and_Set) {
    common::utils::Queue<std::shared_ptr<network::Connection>> q;
    auto conn = create_test_connection(common::enums::operators::CONNECT);
    q.Push(conn);

    TestTaskWorker tw(q, kStoragePath);

    conn->GetPacket()->command = common::enums::operators::SET;
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
    auto conn = create_test_connection(common::enums::operators::CONNECT);
    q.Push(conn);
    MockTaskWorker mockTaskWorker(q, kStoragePath);

    conn->GetPacket()->command = common::enums::operators::SET;

    EXPECT_CALL(mockTaskWorker, Set_(conn->GetPacket()->data)).Times(1);
    EXPECT_EQ(mockTaskWorker.DoTask(conn), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Self_Read) {
    common::utils::Queue<std::shared_ptr<network::Connection>> q;
    auto conn = create_test_connection(common::enums::operators::CONNECT);
    q.Push(conn);
    MockTaskWorker mockTaskWorker(q, kStoragePath);

    conn->GetPacket()->command = common::enums::operators::GET;

    EXPECT_CALL(mockTaskWorker, Read_(conn->GetPacket()->data)).Times(1);
    EXPECT_EQ(mockTaskWorker.DoTask(conn), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Self_Update) {
    common::utils::Queue<std::shared_ptr<network::Connection>> q;
    auto conn = create_test_connection(common::enums::operators::CONNECT);
    q.Push(conn);
    MockTaskWorker mockTaskWorker(q, kStoragePath);

    conn->GetPacket()->command = common::enums::operators::UPDATE;

    EXPECT_CALL(mockTaskWorker, Update_(conn->GetPacket()->data)).Times(1);
    EXPECT_EQ(mockTaskWorker.DoTask(conn), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Self_Delete) {
    common::utils::Queue<std::shared_ptr<network::Connection>> q;
    auto conn = create_test_connection(common::enums::operators::CONNECT);
    q.Push(conn);
    MockTaskWorker mockTaskWorker(q, kStoragePath);

    conn->GetPacket()->command = common::enums::operators::DELETE;

    EXPECT_CALL(mockTaskWorker, Delete_(conn->GetPacket()->data)).Times(1);
    EXPECT_EQ(mockTaskWorker.DoTask(conn), EXIT_SUCCESS);
}

// fails to create thread
TEST(TaskManager, WorkingInThread) {
    common::utils::Queue<std::shared_ptr<network::Connection>> q;
    auto conn = create_test_connection(common::enums::operators::CONNECT);
    q.Push(conn);

    TestTaskWorker tw(q, kStoragePath);

    conn->GetPacket()->command =common::enums::operators::SET;
    q.Push(conn);
    conn->GetPacket()->command = common::enums::operators::GET;
    q.Push(conn);
    conn->GetPacket()->command = common::enums::operators::DELETE;
    q.Push(conn);
    conn->GetPacket()->command = common::enums::operators::KILL;
    q.Push(conn);
}

} // namespace failless::db::tests

#endif // FAILLESS_TESTS_TESTS_DB_TEST_TASK_WORKER_H_
