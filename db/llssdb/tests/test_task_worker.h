#ifndef LLSSDB_TESTS_TEST_TASK_WORKER_H
#define LLSSDB_TESTS_TEST_TASK_WORKER_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/folder/task.h"
#include "llssdb/folder/task_worker.h"

namespace failless::db::tests {

using ::testing::_;
using ::testing::AtLeast;

class MockTaskWorker : public ITaskWorker {
public:
    MockTaskWorker() : ITaskWorker() {};
    MOCK_METHOD0(IsEmpty, bool());
    MOCK_METHOD0(Create, void());
    MOCK_METHOD0(Read, void());
    MOCK_METHOD0(Update, void());
    MOCK_METHOD0(Delete, void());
};

TEST(ITaskManager, Create) {
    MockTaskWorker mockTaskWorker;
    EXPECT_CALL(mockTaskWorker, Create()).Times(AtLeast(1));

    Task temp_task(Task::CREATE, 0);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

TEST(ITaskManager, Read) {
    MockTaskWorker mockTaskWorker;
    EXPECT_CALL(mockTaskWorker, Read()).Times(AtLeast(1));

    Task temp_task(Task::READ, 0);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

TEST(ITaskManager, Update) {
    MockTaskWorker mockTaskWorker;
    EXPECT_CALL(mockTaskWorker, Update()).Times(AtLeast(1));

    Task temp_task(Task::UPDATE, 0);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

TEST(ITaskManager, Delete) {
    MockTaskWorker mockTaskWorker;
    EXPECT_CALL(mockTaskWorker, Delete()).Times(AtLeast(1));

    Task temp_task(Task::DELETE, 0);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

} // namespace failless::db::tests

#endif // LLSSDB_TESTS_TEST_TASK_WORKER_H
