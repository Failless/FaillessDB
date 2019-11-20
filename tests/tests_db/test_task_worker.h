#ifndef TESTS_DB_TEST_TASK_WORKER_H_
#define TESTS_DB_TEST_TASK_WORKER_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/folder/task_worker.h"

namespace failless::db::tests {

using ::testing::_;
using ::testing::AtLeast;

class MockTaskWorker : public folder::TaskWorker {
 public:
    MockTaskWorker() : TaskWorker(){};
    MOCK_METHOD0(IsEmpty, bool());
    MOCK_METHOD0(CompleteTask, int());
};

TEST(ITaskManager, AddTask) {
    auto *dw = new DataWorker;
    MockTaskWorker mockTaskWorker;
    EXPECT_CALL(mockTaskWorker, IsEmpty()).Times(AtLeast(1));
    EXPECT_CALL(mockTaskWorker, CompleteTask()).Times(AtLeast(1));
    std::string query = "trash";
    common::Task temp_task(common::operators::CREATE, &query);
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

}  // namespace failless::db::tests

#endif  // TESTS_DB_TEST_TASK_WORKER_H_
