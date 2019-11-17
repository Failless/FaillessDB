#ifndef LLSSDB_TESTS_TEST_TASK_WORKER_H
#define LLSSDB_TESTS_TEST_TASK_WORKER_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/folder/task.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/task_worker.h"

namespace failless::db::tests {

using ::testing::_;

class MockTaskWorker : public TaskWorker {
public:
    MockTaskWorker(FileSystem* _fs) : TaskWorker(_fs) {};
    MOCK_METHOD0(IsEmpty, bool());
    MOCK_METHOD1(Create, bool(const u_int8_t& data));
    MOCK_METHOD1(Read, bool(const u_int8_t& data));
    MOCK_METHOD1(Update, bool(const u_int8_t& data));
    MOCK_METHOD1(Delete, bool(const u_int8_t& data));
};

TEST(TaskManager, Create) {
    MockTaskWorker mockTaskWorker(nullptr);
    u_int8_t data = 0;
    Task temp_task(Task::CREATE, data);

    EXPECT_CALL(mockTaskWorker, Create(data)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}


TEST(TaskManager, Read) {
    MockTaskWorker mockTaskWorker(nullptr);
    u_int8_t data = 0;
    Task temp_task(Task::READ, data);

    EXPECT_CALL(mockTaskWorker, Read(data)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

TEST(TaskManager, Update) {
    MockTaskWorker mockTaskWorker(nullptr);
    u_int8_t data = 0;
    Task temp_task(Task::UPDATE, data);

    EXPECT_CALL(mockTaskWorker, Update(data)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

TEST(TaskManager, Delete) {
    MockTaskWorker mockTaskWorker(nullptr);
    u_int8_t data = 0;
    Task temp_task(Task::DELETE, data);

    EXPECT_CALL(mockTaskWorker, Delete(data)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

TEST(TaskManager, CREATE_Calling_Set) {
    MockFileSystem mockFileSystem;
    u_int8_t data = 0;
    int key = 0;
    Task temp_task(Task::CREATE, data);

    EXPECT_CALL(mockFileSystem, Set(key, data)).Times(1);

    TaskWorker taskWorker(&mockFileSystem);
    taskWorker.AddTask(temp_task);
}

TEST(TaskManager, READ_Calling_Get) {
    MockFileSystem mockFileSystem;
    u_int8_t data = 0;
    int key = 0;
    Task temp_task(Task::READ, data);

    EXPECT_CALL(mockFileSystem, Get(key)).Times(1);

    TaskWorker taskWorker(&mockFileSystem);
    taskWorker.AddTask(temp_task);
}

TEST(TaskManager, UPDATE_Calling_Set) {
    MockFileSystem mockFileSystem;
    u_int8_t data = 0;
    int key = 0;
    Task temp_task(Task::UPDATE, data);

    EXPECT_CALL(mockFileSystem, Set(key, data)).Times(1);

    TaskWorker taskWorker(&mockFileSystem);
    taskWorker.AddTask(temp_task);
}

TEST(TaskManager, DELETE_Calling_Remove) {
    MockFileSystem mockFileSystem;
    u_int8_t data = 0;
    Task temp_task(Task::DELETE, data);

    EXPECT_CALL(mockFileSystem, Remove(data)).Times(1);

    TaskWorker taskWorker(&mockFileSystem);
    taskWorker.AddTask(temp_task);
}

} // namespace failless::db::tests

#endif // LLSSDB_TESTS_TEST_TASK_WORKER_H
