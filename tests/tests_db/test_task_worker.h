#ifndef TESTS_DB_TEST_TASK_WORKER_H_
#define TESTS_DB_TEST_TASK_WORKER_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/task_worker.h"
#include "llssdb/utils/task.h"
///////////////////////////////////////////////////////////////////////////////////
/// README:                                                                     ///
/// If you build this project via CLion:                                        ///
///      1. Choose first path                                                   ///
///      2. Comment second one                                                  ///
///      3. Create next empty folders manually:                                 ///
///          cmake-build-debug/llssdb/CMakeFiles/llssdb.dir/storage             ///
///          cmake-build-debug/llssdb/CMakeFiles/llssdb.dir/storage/test_user   ///
///                                                                             ///
/// If you build this project via "cmake . && make":                            ///
///      1. Choose second path                                                  ///
///      2. Comment first one                                                   ///
///////////////////////////////////////////////////////////////////////////////////

const std::string test_db_path = "llssdb/CMakeFiles/llssdb.dir/storage/test_user";
// std::string test_db_path = "./llssdb/storage/test_user";

namespace failless::db::tests {

using folder::TaskWorker;
using ::testing::_;

class MockTaskWorker : public TaskWorker {
 public:
    explicit MockTaskWorker(const std::string &db_path = test_db_path) : TaskWorker(db_path){};
    MOCK_METHOD1(Set, bool(const utils::Task &task_in));
    MOCK_METHOD1(Read, bool(const utils::Task &task_in));
    MOCK_METHOD1(Update, bool(const utils::Task &task_in));
    MOCK_METHOD1(Delete, bool(const utils::Task &task_in));
};

TEST(TaskManager, Get_and_Set) {
    TaskWorker tw(test_db_path);
    size_t size = 3;
    auto value1 = new int8_t[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value1[iii] = iii;
    }
    auto *key1 = new std::string("test_key");
    utils::Task test_task1(value1, size, key1, common::enums::operators::SET);
    bool result1 = tw.AddTask(test_task1);
    test_task1.Destruct();

    auto value2 = new int8_t[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value2[iii] = iii;
    }
    auto *key2 = new std::string("test_key");
    utils::Task test_task2(value2, size, key2, common::enums::operators::GET);
    bool result2 = tw.AddTask(test_task2);
    test_task2.Destruct();

    EXPECT_EQ(result1, result2);

    // clear
    auto value3 = new int8_t[size];
    for (size_t iii = 0; iii < size; ++iii) value3[iii] = iii;
    auto *key3 = new std::string("test_key");
    utils::Task test_task3(value3, size, key3, common::enums::operators::DELETE);
    tw.AddTask(test_task3);
    test_task3.Destruct();
}

// mocktests stopped working since they are calling destructor twice

TEST(TaskManager, Calling_Set) {
    MockTaskWorker mockTaskWorker(test_db_path);
    auto *key = new std::string("test_key");
    size_t size = 3;
    auto value = new int8_t[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value[iii] = iii;
    }
    utils::Task test_task(value, size, key, common::enums::operators::SET);

    EXPECT_CALL(mockTaskWorker, Set(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
    test_task.Destruct();
}

TEST(TaskManager, Calling_Read) {
    MockTaskWorker mockTaskWorker(test_db_path);
    auto *key = new std::string("test_key");
    size_t size = 3;
    auto value = new int8_t[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value[iii] = iii;
    }
    utils::Task test_task(value, size, key, common::enums::operators::GET);

    EXPECT_CALL(mockTaskWorker, Read(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
    test_task.Destruct();
}

TEST(TaskManager, Calling_Update) {
    MockTaskWorker mockTaskWorker(test_db_path);
    auto *key = new std::string("test_key");
    size_t size = 3;
    auto value = new int8_t[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value[iii] = iii;
    }
    utils::Task test_task(value, size, key, common::enums::operators::UPDATE);

    EXPECT_CALL(mockTaskWorker, Update(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
    test_task.Destruct();
}

TEST(TaskManager, Calling_Delete) {
    MockTaskWorker mockTaskWorker(test_db_path);
    auto *key = new std::string("test_key");
    size_t size = 3;
    auto value = new int8_t[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value[iii] = iii;
    }
    utils::Task test_task(value, size, key, common::enums::operators::DELETE);

    EXPECT_CALL(mockTaskWorker, Delete(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
    test_task.Destruct();
}

/// test for loading from local_Storage

}  // namespace failless::db::tests

#endif  // LLSSDB_TESTS_TEST_TASK_WORKER_H_
