#ifndef TESTS_TESTS_DB_TEST_TASK_WORKER_H_
#define TESTS_TESTS_DB_TEST_TASK_WORKER_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

#include "llssdb/utils/task.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/task_worker.h"
#include "tests/tests_db/mocks.h"
#include "tests/tests_db/test_file_system.h"

/*
namespace failless::db::tests {

<<<<<<< HEAD
TEST(TaskManager, Get_and_Set) {
    folder::TaskWorker tw(kTestDbPath);

    size_t size = 3;
    auto key = std::make_shared<std::string>("test_key");
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});
    utils::Task test_task1(value, size, key, utils::operators::SET);
    bool result1 = tw.AddTask(test_task1);

    utils::Task test_task2(nullptr, size, key, utils::operators::GET);
=======
using folder::TaskWorker;
using ::testing::_;
class MockTaskWorker : public TaskWorker {
 public:
    explicit MockTaskWorker(const std::string &db_path = kTestDbPath) : TaskWorker(db_path){};
    MOCK_METHOD1(Set, bool(const utils::Task &task_in));
    MOCK_METHOD1(Read, bool(const utils::Task &task_in));
    MOCK_METHOD1(Update, bool(const utils::Task &task_in));
    MOCK_METHOD1(Delete, bool(const utils::Task &task_in));
};

TEST(TaskManager, Get_and_Set) {
    TaskWorker tw(kTestDbPath);
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
>>>>>>> feature/network
    bool result2 = tw.AddTask(test_task2);

    EXPECT_EQ(result1, result2);

    // clear
<<<<<<< HEAD
    utils::Task test_task3(nullptr, size, key, utils::operators::DELETE);
=======
    uint8_t value3[kSize];
    for (size_t iii = 0; iii < kSize; ++iii) {
        value3[iii] = iii;
    }
    std::string key3("test_key");
    utils::Task test_task3(value3, kSize, &key3, common::enums::operators::DELETE);
>>>>>>> feature/network
    tw.AddTask(test_task3);
}

TEST(TaskManager, Calling_Self_Set) {
    MockTaskWorker mockTaskWorker(kTestDbPath);
<<<<<<< HEAD

    size_t size = 3;
    auto key = std::make_shared<std::string>("test_key");
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});

    utils::Task test_task(value, size, key, utils::operators::SET);
=======
    std::string key("test_key");
    size_t size = 3;
    uint8_t value[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value[iii] = iii;
    }
    utils::Task test_task(value, size, &key, common::enums::operators::SET);
>>>>>>> feature/network

    EXPECT_CALL(mockTaskWorker, Set(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}


TEST(TaskManager, Calling_Self_Read) {
    MockTaskWorker mockTaskWorker(kTestDbPath);
<<<<<<< HEAD

    size_t size = 0;
    auto key = std::make_shared<std::string>("test_key");

    utils::Task test_task(nullptr, size, key, utils::operators::GET);
=======
    std::string key("test_key");
    size_t size = 3;
    uint8_t value[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value[iii] = iii;
    }
    utils::Task test_task(value, size, &key, common::enums::operators::GET);
>>>>>>> feature/network

    EXPECT_CALL(mockTaskWorker, Read(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Self_Update) {
    MockTaskWorker mockTaskWorker(kTestDbPath);
<<<<<<< HEAD

    size_t size = 3;
    auto key = std::make_shared<std::string>("test_key");
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});

    utils::Task test_task(value, size, key, utils::operators::UPDATE);
=======
    std::string key("test_key");
    size_t size = 3;
    uint8_t value[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value[iii] = iii;
    }
    utils::Task test_task(value, size, &key, common::enums::operators::UPDATE);
>>>>>>> feature/network

    EXPECT_CALL(mockTaskWorker, Update(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Self_Delete) {
    MockTaskWorker mockTaskWorker(kTestDbPath);
<<<<<<< HEAD

    size_t size = 3;
    auto key = std::make_shared<std::string>("test_key");
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});

    utils::Task test_task(value, size, key, utils::operators::DELETE);
=======
    std::string key("test_key");
    size_t size = 3;
    uint8_t value[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value[iii] = iii;
    }
    utils::Task test_task(value, size, &key, common::enums::operators::DELETE);
>>>>>>> feature/network

    EXPECT_CALL(mockTaskWorker, Delete(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}

/// test for loading from local_Storage
} // namespace failless::db::tests
*/


#endif // TESTS_TESTS_DB_TEST_TASK_WORKER_H
