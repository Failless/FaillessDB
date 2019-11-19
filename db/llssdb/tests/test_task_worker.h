#ifndef LLSSDB_TESTS_TEST_TASK_WORKER_H
#define LLSSDB_TESTS_TEST_TASK_WORKER_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/folder/task.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/task_worker.h"

namespace failless::db::tests {

using ::testing::_;
using folder::TaskWorker;

class MockTaskWorker : public TaskWorker {
public:
    explicit MockTaskWorker(FileSystem* _fs) : TaskWorker(_fs) {};
    MOCK_METHOD0(IsEmpty, bool());
    MOCK_METHOD1(Create, bool(const int8_t& value));
    MOCK_METHOD1(Read, bool(const int8_t& value));
    MOCK_METHOD1(Update, bool(const int8_t& value));
    MOCK_METHOD1(Delete, bool(const int8_t& value));
};

TEST(TaskManager, Create) {
    MockTaskWorker mockTaskWorker(nullptr);
    int8_t value = 0;
    Task temp_task(Task::CREATE, value);

    EXPECT_CALL(mockTaskWorker, Create(value)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}


TEST(TaskManager, Read) {
    MockTaskWorker mockTaskWorker(nullptr);
    int8_t value = 0;
    Task temp_task(Task::READ, value);

    EXPECT_CALL(mockTaskWorker, Read(value)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

TEST(TaskManager, Update) {
    MockTaskWorker mockTaskWorker(nullptr);
    int8_t value = 0;
    Task temp_task(Task::UPDATE, value);

    EXPECT_CALL(mockTaskWorker, Update(value)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

TEST(TaskManager, Delete) {
    MockTaskWorker mockTaskWorker(nullptr);
    int8_t value = 0;
    Task temp_task(Task::DELETE, value);

    EXPECT_CALL(mockTaskWorker, Delete(value)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

TEST(TaskManager, CREATE_Calling_Set) {
    MockFileSystem mockFileSystem(test_db_path);
    int8_t value = 0;
    std::string key = "0";
    Task temp_task(Task::CREATE, value);

    EXPECT_CALL(mockFileSystem, Set(key, value)).Times(1);

    TaskWorker taskWorker(&mockFileSystem);
    taskWorker.AddTask(temp_task);
}

TEST(TaskManager, READ_Calling_Get) {
    MockFileSystem mockFileSystem(test_db_path);
    int8_t value = 0;
    string key = 0;
    Task temp_task(Task::READ, value);

    EXPECT_CALL(mockFileSystem, Get(key)).Times(1);

    TaskWorker taskWorker(&mockFileSystem);
    taskWorker.AddTask(temp_task);
}

TEST(TaskManager, UPDATE_Calling_Set) {
    MockFileSystem mockFileSystem(test_db_path);
    int8_t value = 0;
    string key = "0";
    Task temp_task(Task::UPDATE, value);

    EXPECT_CALL(mockFileSystem, Set(key, value)).Times(1);

    TaskWorker taskWorker(&mockFileSystem);
    taskWorker.AddTask(temp_task);
}

TEST(TaskManager, DELETE_Calling_Remove) {
    MockFileSystem mockFileSystem(test_db_path);
    int8_t value = 0;
    string key = "0";
    Task temp_task(Task::DELETE, value);

    EXPECT_CALL(mockFileSystem, Remove(key)).Times(1);

    TaskWorker taskWorker(&mockFileSystem);
    taskWorker.AddTask(temp_task);
}

} // namespace failless::db::tests

#endif // LLSSDB_TESTS_TEST_TASK_WORKER_H
