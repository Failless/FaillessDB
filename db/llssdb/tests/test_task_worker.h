#ifndef LLSSDB_TESTS_TEST_TASK_WORKER_H
#define LLSSDB_TESTS_TEST_TASK_WORKER_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/folder/task.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/task_worker.h"

namespace failless::db::tests {

using ::testing::_;
using ::testing::AtLeast;

class MockTaskWorker : public TaskWorker {
public:
    MockTaskWorker() : TaskWorker() {};
    MOCK_METHOD0(IsEmpty, bool());
    MOCK_METHOD1(Create, bool(const u_int8_t& data));
    MOCK_METHOD1(Read, bool(const u_int8_t& data));
    MOCK_METHOD1(Update, bool(const u_int8_t& data));
    MOCK_METHOD1(Delete, bool(const u_int8_t& data));
};

TEST(TaskManager, Create) {
    MockFileSystem mockFileSystem;
    const u_int8_t& data = 0;
    int key = 0;
    Task temp_task(Task::CREATE, data);

//    EXPECT_CALL(TaskWorker, Create(data)).Times(AtLeast(1));
    TaskWorker taskWorker(&mockFileSystem);
    EXPECT_CALL(mockFileSystem, Set(key, data)).Times(AtLeast(1));
    EXPECT_EQ(TaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

TEST(TaskManager, Read) {
    MockTaskWorker mockTaskWorker;
    const u_int8_t& data = 0;
    EXPECT_CALL(mockTaskWorker, Read(data)).Times(AtLeast(1));

    Task temp_task(Task::READ, data);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

TEST(ITaskManager, Update) {
    MockTaskWorker mockTaskWorker;
    const u_int8_t& data = 0;
    EXPECT_CALL(mockTaskWorker, Update(data)).Times(AtLeast(1));

    Task temp_task(Task::UPDATE, data);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

TEST(ITaskManager, Delete) {
    MockTaskWorker mockTaskWorker;
    const u_int8_t& data = 0;
    EXPECT_CALL(mockTaskWorker, Delete(data)).Times(AtLeast(1));

    Task temp_task(Task::DELETE, data);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

} // namespace failless::db::tests

#endif // LLSSDB_TESTS_TEST_TASK_WORKER_H
