#ifndef LLSSDB_TESTS_TEST_TASK_WORKER_H
#define LLSSDB_TESTS_TEST_TASK_WORKER_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/folder/task.h"
#include "llssdb/folder/task_worker.h"

namespace failless::db::tests {

using ::testing::_;
using ::testing::AtLeast;

class MockTaskWorker : public ITaskWorker {
public:
    explicit MockTaskWorker(DataWorker *_data_worker) : ITaskWorker(_data_worker) {};
    MOCK_METHOD0(IsEmpty, bool());
    MOCK_METHOD0(CompleteTask, int());
};

TEST(ITaskManager, AddTask) {
    auto *dw = new DataWorker;
    MockTaskWorker mockTaskWorker(dw);
    EXPECT_CALL(mockTaskWorker, IsEmpty()).Times(AtLeast(1));
    EXPECT_CALL(mockTaskWorker, CompleteTask()).Times(AtLeast(1));
    Task temp_task(Task::CREATE, "trash");
    EXPECT_EQ(mockTaskWorker.AddTask(temp_task), EXIT_SUCCESS);
}

} // namespace failless

#endif //LLSSDB_TESTS_TEST_TASK_WORKER_H
