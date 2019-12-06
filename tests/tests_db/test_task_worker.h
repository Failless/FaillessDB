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
/*
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
    const size_t kSize = 3;
    uint8_t value1[kSize];
    for (size_t iii = 0; iii < kSize; ++iii) {
        value1[iii] = iii;
    }
    std::string key1("test_key");
    utils::Task test_task1(value1, kSize, &key1, common::enums::operators::SET);
    bool result1 = tw.AddTask(test_task1);
    test_task1.Destruct();

    uint8_t value2[kSize];
    for (size_t iii = 0; iii < kSize; ++iii) {
        value2[iii] = iii;
    }
    std::string key2("test_key");
    utils::Task test_task2(value2, kSize, &key2, common::enums::operators::GET);
    bool result2 = tw.AddTask(test_task2);
    test_task2.Destruct();

    EXPECT_EQ(result1, result2);

    // clear
    uint8_t value3[kSize];
    for (size_t iii = 0; iii < kSize; ++iii) {
        value3[iii] = iii;
    }
    std::string key3("test_key");
    utils::Task test_task3(value3, kSize, &key3, common::enums::operators::DELETE);
    tw.AddTask(test_task3);
    test_task3.Destruct();
}

// mocktests stopped working since they are calling destructor twice

TEST(TaskManager, Calling_Set) {
    MockTaskWorker mockTaskWorker(test_db_path);
    std::string key("test_key");
    size_t size = 3;
    uint8_t value[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value[iii] = iii;
    }
    utils::Task test_task(value, size, &key, common::enums::operators::SET);

    EXPECT_CALL(mockTaskWorker, Set(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
    test_task.Destruct();
}

TEST(TaskManager, Calling_Read) {
    MockTaskWorker mockTaskWorker(test_db_path);
    std::string key("test_key");
    size_t size = 3;
    uint8_t value[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value[iii] = iii;
    }
    utils::Task test_task(value, size, &key, common::enums::operators::GET);

    EXPECT_CALL(mockTaskWorker, Read(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
    test_task.Destruct();
}

TEST(TaskManager, Calling_Update) {
    MockTaskWorker mockTaskWorker(test_db_path);
    std::string key("test_key");
    size_t size = 3;
    uint8_t value[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value[iii] = iii;
    }
    utils::Task test_task(value, size, &key, common::enums::operators::UPDATE);

    EXPECT_CALL(mockTaskWorker, Update(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
    test_task.Destruct();
}

TEST(TaskManager, Calling_Delete) {
    MockTaskWorker mockTaskWorker(test_db_path);
    std::string key("test_key");
    size_t size = 3;
    uint8_t value[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value[iii] = iii;
    }
    utils::Task test_task(value, size, &key, common::enums::operators::DELETE);

    EXPECT_CALL(mockTaskWorker, Delete(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
    test_task.Destruct();
}

/// test for loading from local_Storage
*/

}  // namespace failless::db::tests

#endif  // LLSSDB_TESTS_TEST_TASK_WORKER_H_
