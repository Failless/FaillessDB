#ifndef TESTS_TESTS_DB_TEST_TASK_WORKER_H_
#define TESTS_TESTS_DB_TEST_TASK_WORKER_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include "llssdb/common/task.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/task_worker.h"
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
//const std::string test_db_path = "./llssdb/storage/test_user";

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
    MOCK_METHOD0(LoadInMemory, void());
    MOCK_METHOD0(UnloadFromMemory, void());
};

TEST(TaskManager, Get_and_Set) {
    TaskWorker tw(test_db_path);

    size_t size = 3;
    auto key = std::make_shared<std::string>("test_key");
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});
    common::Task test_task1(value, size, key, common::operators::SET);
    bool result1 = tw.AddTask(test_task1);

    common::Task test_task2(nullptr, size, key, common::operators::GET);
    bool result2 = tw.AddTask(test_task2);

    EXPECT_EQ(result1, result2);

    // clear
    common::Task test_task3(nullptr, size, key, common::operators::DELETE);
    tw.AddTask(test_task3);
}

TEST(TaskManager, Calling_Set) {
    MockTaskWorker mockTaskWorker(test_db_path);

    size_t size = 3;
    auto key = std::make_shared<std::string>("test_key");
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});

    common::Task test_task(value, size, key, common::operators::SET);

    EXPECT_CALL(mockTaskWorker, Set(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}


TEST(TaskManager, Calling_Read) {
    MockTaskWorker mockTaskWorker(test_db_path);

    size_t size = 0;
    auto key = std::make_shared<std::string>("test_key");

    common::Task test_task(nullptr, size, key, common::operators::GET);

    EXPECT_CALL(mockTaskWorker, Read(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Update) {
    MockTaskWorker mockTaskWorker(test_db_path);

    size_t size = 3;
    auto key = std::make_shared<std::string>("test_key");
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});

    common::Task test_task(value, size, key, common::operators::UPDATE);

    EXPECT_CALL(mockTaskWorker, Update(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Delete) {
    MockTaskWorker mockTaskWorker(test_db_path);

    size_t size = 3;
    auto key = std::make_shared<std::string>("test_key");
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});

    common::Task test_task(value, size, key, common::operators::DELETE);

    EXPECT_CALL(mockTaskWorker, Delete(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}

//TEST(TaskManager, Calling_)

} // namespace failless::db::tests

#endif // TESTS_TESTS_DB_TEST_TASK_WORKER_H
