#ifndef TESTS_DB_TEST_TASK_WORKER_H_
#define TESTS_DB_TEST_TASK_WORKER_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include "llssdb/common/task.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/task_worker.h"

// this is the path for debug testing
const std::string test_db_path = "llssdb/CMakeFiles/llssdb.dir/storage/test_user";

namespace failless::db::tests {

using ::testing::_;
using folder::TaskWorker;

class MockTaskWorker : public TaskWorker {
public:
    explicit MockTaskWorker(const std::string& db_path = test_db_path) : TaskWorker(db_path) {};
    MOCK_METHOD1(Set, bool(const common::Task &task_in));
    MOCK_METHOD1(Read, bool(const common::Task &task_in));
    MOCK_METHOD1(Update, bool(const common::Task &task_in));
    MOCK_METHOD1(Delete, bool(const common::Task &task_in));
};

TEST(TaskManager, Calling_Set) {
    MockTaskWorker mockTaskWorker(test_db_path);
    std::string key = "test_key";
    size_t size = 3;
    auto value = new int8_t[size];
    for ( size_t iii = 0; iii < size; ++iii ) {
        value[iii] = iii;
    }
    common::Task test_task(value, size, key);

    EXPECT_CALL(mockTaskWorker, Set(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task, common::operators::SET), EXIT_SUCCESS);
}


TEST(TaskManager, Calling_Read) {
    MockTaskWorker mockTaskWorker(test_db_path);
    std::string key = "test_key";
    size_t size = 3;
    auto value = new int8_t[size];
    for ( size_t iii = 0; iii < size; ++iii ) {
        value[iii] = iii;
    }
    common::Task test_task(value, size, key);

    EXPECT_CALL(mockTaskWorker, Read(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task, common::operators::GET), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Update) {
    MockTaskWorker mockTaskWorker(test_db_path);
    std::string key = "test_key";
    size_t size = 3;
    auto value = new int8_t[size];
    for ( size_t iii = 0; iii < size; ++iii ) {
        value[iii] = iii;
    }
    common::Task test_task(value, size, key);

    EXPECT_CALL(mockTaskWorker, Update(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task, common::operators::UPDATE), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Delete) {
    MockTaskWorker mockTaskWorker(test_db_path);
    std::string key = "test_key";
    size_t size = 3;
    auto value = new int8_t[size];
    for ( size_t iii = 0; iii < size; ++iii ) {
        value[iii] = iii;
    }
    common::Task test_task(value, size, key);

    EXPECT_CALL(mockTaskWorker, Delete(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task, common::operators::DELETE), EXIT_SUCCESS);
}

TEST(TaskManager, Get_and_Set) {
    TaskWorker tw(test_db_path);
    std::string key = "test_key";
    size_t size = 3;
    auto value = new int8_t[size];
    for ( size_t iii = 0; iii < size; ++iii ) {
        value[iii] = iii;
    }
    common::Task test_task(value, size, key);

    bool result1 = tw.AddTask(test_task, common::operators::SET);
    bool result2 = tw.AddTask(test_task, common::operators::GET);
    EXPECT_EQ(result1, result2);

    // actually i should clear after tests
    tw.AddTask(test_task, common::operators::DELETE);
}

/// test for loading from local_Storage

} // namespace failless::db::tests

#endif // LLSSDB_TESTS_TEST_TASK_WORKER_H
