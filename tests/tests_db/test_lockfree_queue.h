#ifndef FAILLESS_TESTS_TESTS_DB_SIMPLE_TEST_LOCKFREE_QUEUE_H_
#define FAILLESS_TESTS_TESTS_DB_SIMPLE_TEST_LOCKFREE_QUEUE_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <llss3p/utils/queue.h>
#include <thread>

using ::testing::_;
using ::testing::AtLeast;

namespace failless {
namespace db {
namespace tests {

class IUseQueue {
 public:
    explicit IUseQueue(common::utils::Queue<int>& queue) : queue_(queue) {}
    void PushTasks() {
        for (int i = 0; i < 1000; ++i) {
            queue_.Push(i);
        }
    }
    void PushOtherTasks() {
        for (int i = 1000; i < 2000; ++i) {
            queue_.Push(i);
        }
    }
 private:
    common::utils::Queue<int>& queue_;
};

void RunQueueWorker(IUseQueue user) {
    user.PushTasks();
}
void RunQueueOtherWorker(IUseQueue user) {
    user.PushOtherTasks();
}

TEST(Queue, PushPop) {
    common::utils::Queue<int> queue;
    int kTestNum = 100;
    int test_arr[kTestNum];
    for (int i = 0; i < kTestNum; ++i) {
        test_arr[i] = i;
        queue.Push(i);
    }
    int i = 0;
    while (!queue.IsEmpty() || i < kTestNum) {
        int item = queue.Pop();
        EXPECT_EQ(item, test_arr[i]);
        ++i;
    }
    EXPECT_EQ(i, kTestNum);
    EXPECT_TRUE(queue.IsEmpty());
}

TEST(Queue, ThreadSafe) {
    common::utils::Queue<int> queue;
    std::queue<int> normal_queue;
    IUseQueue user1(queue);
    IUseQueue user2(queue);
    std::thread init1(RunQueueWorker, user1);
    std::thread init2(RunQueueOtherWorker, user2);
    EXPECT_EQ(0, queue.Pop());
    while (!queue.IsEmpty()) {
        queue.Pop();
    }
    init1.join();
    init2.join();
}


}  // namespace tests
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_TESTS_TESTS_DB_SIMPLE_TEST_LOCKFREE_QUEUE_H_
