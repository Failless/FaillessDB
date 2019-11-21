#ifndef TESTS_DB_TEST_TASK_WORKER_H_
#define TESTS_DB_TEST_TASK_WORKER_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
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
    MOCK_METHOD1(Create, bool(const common::Task &task));
    MOCK_METHOD1(Read, bool(const common::Task &task));
    MOCK_METHOD1(Update, bool(const common::Task &task));
    MOCK_METHOD1(Delete, bool(const common::Task &task));
};

TEST(TaskManager, Calling_Create) {
    MockTaskWorker mockTaskWorker(test_db_path);
    int8_t value[3] = {1, 2, 3};
    common::Task test_task(common::operators::CREATE, value, 3);

    EXPECT_CALL(mockTaskWorker, Create(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}


TEST(TaskManager, Calling_Read) {
    MockTaskWorker mockTaskWorker(test_db_path);
    int8_t value[3] = {1, 2, 3};
    common::Task test_task(common::operators::GET, value, 3);

    EXPECT_CALL(mockTaskWorker, Read(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Update) {
    MockTaskWorker mockTaskWorker(test_db_path);
    int8_t value[3] = {1, 2, 3};
    common::Task test_task(common::operators::UPDATE, value, 3);

    EXPECT_CALL(mockTaskWorker, Update(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}

TEST(TaskManager, Calling_Delete) {
    MockTaskWorker mockTaskWorker(test_db_path);
    int8_t value[3] = {1, 2, 3};
    common::Task test_task(common::operators::DELETE, value, 3);

    EXPECT_CALL(mockTaskWorker, Delete(test_task)).Times(1);
    EXPECT_EQ(mockTaskWorker.AddTask(test_task), EXIT_SUCCESS);
}

} // namespace failless::db::tests

#endif // LLSSDB_TESTS_TEST_TASK_WORKER_H
