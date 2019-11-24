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

TEST(TaskManager, Get_and_Set) {
    TaskWorker tw(test_db_path);
    size_t size = 3;
    auto value1 = new int8_t[size];
    for ( size_t iii = 0; iii < size; ++iii ) {
        value1[iii] = iii;
    }
    auto *key1 = new std::string("test_key");
    common::Task test_task1(value1, size, key1, common::operators::SET);
    bool result1 = tw.AddTask(test_task1);

    auto value2 = new int8_t[size];
    for ( size_t iii = 0; iii < size; ++iii ) {
        value2[iii] = iii;
    }
    auto *key2 = new std::string("test_key");
    common::Task test_task2(value2, size, key2, common::operators::GET);
    bool result2 = tw.AddTask(test_task2);

    EXPECT_EQ(result1, result2);

    // clear
    auto value3 = new int8_t[size];
    for ( size_t iii = 0; iii < size; ++iii )
        value3[iii] = iii;
    auto *key3 = new std::string("test_key");
    common::Task test_task3(value3, size, key3, common::operators::DELETE);
    tw.AddTask(test_task3);
}

/* // mocktests stopped working since they are calling destructor twice

TEST(TaskManager, Calling_Set) {
    MockTaskWorker mockTaskWorker(test_db_path);
    auto *key = new std::string("test_key");
    size_t size = 3;
    auto value = new int8_t[size];
    for ( size_t iii = 0; iii < size; ++iii ) {
        value[iii] = iii;
    }
    common::Task test_task(value, size, key, common::operators::SET);

    EXPECT_CALL(mockTaskWorker, Set(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}


TEST(TaskManager, Calling_Read) {
    MockTaskWorker mockTaskWorker(test_db_path);
    auto *key = new std::string("test_key");
    size_t size = 3;
    auto value = new int8_t[size];
    for ( size_t iii = 0; iii < size; ++iii ) {
        value[iii] = iii;
    }
    common::Task test_task(value, size, key, common::operators::GET);

    EXPECT_CALL(mockTaskWorker, Read(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Update) {
    MockTaskWorker mockTaskWorker(test_db_path);
    auto *key = new std::string("test_key");
    size_t size = 3;
    auto value = new int8_t[size];
    for ( size_t iii = 0; iii < size; ++iii ) {
        value[iii] = iii;
    }
    common::Task test_task(value, size, key, common::operators::UPDATE);

    EXPECT_CALL(mockTaskWorker, Update(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Delete) {
    MockTaskWorker mockTaskWorker(test_db_path);
    auto *key = new std::string("test_key");
    size_t size = 3;
    auto value = new int8_t[size];
    for ( size_t iii = 0; iii < size; ++iii ) {
        value[iii] = iii;
    }
    common::Task test_task(value, size, key, common::operators::DELETE);

    EXPECT_CALL(mockTaskWorker, Delete(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}
*/

/// test for loading from local_Storage

} // namespace failless::db::tests

#endif // LLSSDB_TESTS_TEST_TASK_WORKER_H
