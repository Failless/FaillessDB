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

namespace failless::db::tests {

TEST(TaskManager, Get_and_Set) {
    folder::TaskWorker tw(test_db_path);

    size_t size = 3;
    auto key = std::make_shared<std::string>("test_key");
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});
    utils::Task test_task1(value, size, key, utils::operators::SET);
    bool result1 = tw.AddTask(test_task1);

    utils::Task test_task2(nullptr, size, key, utils::operators::GET);
    bool result2 = tw.AddTask(test_task2);

    EXPECT_EQ(result1, result2);

    // clear
    utils::Task test_task3(nullptr, size, key, utils::operators::DELETE);
    tw.AddTask(test_task3);
}

TEST(TaskManager, Calling_Self_Set) {
    MockTaskWorker mockTaskWorker(test_db_path);

    size_t size = 3;
    auto key = std::make_shared<std::string>("test_key");
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});

    utils::Task test_task(value, size, key, utils::operators::SET);

    EXPECT_CALL(mockTaskWorker, Set(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}


TEST(TaskManager, Calling_Self_Read) {
    MockTaskWorker mockTaskWorker(test_db_path);

    size_t size = 0;
    auto key = std::make_shared<std::string>("test_key");

    utils::Task test_task(nullptr, size, key, utils::operators::GET);

    EXPECT_CALL(mockTaskWorker, Read(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Self_Update) {
    MockTaskWorker mockTaskWorker(test_db_path);

    size_t size = 3;
    auto key = std::make_shared<std::string>("test_key");
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});

    utils::Task test_task(value, size, key, utils::operators::UPDATE);

    EXPECT_CALL(mockTaskWorker, Update(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Self_Delete) {
    MockTaskWorker mockTaskWorker(test_db_path);

    size_t size = 3;
    auto key = std::make_shared<std::string>("test_key");
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});

    utils::Task test_task(value, size, key, utils::operators::DELETE);

    EXPECT_CALL(mockTaskWorker, Delete(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}

} // namespace failless::db::tests

#endif // TESTS_TESTS_DB_TEST_TASK_WORKER_H
