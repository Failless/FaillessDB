#ifndef FAILLESS_TESTS_TESTS_DB_SIMPLE_TEST_LOCKFREE_QUEUE_H_
#define FAILLESS_TESTS_TESTS_DB_SIMPLE_TEST_LOCKFREE_QUEUE_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <llss3p/utils/queue.h>

using ::testing::_;
using ::testing::AtLeast;

namespace failless {
namespace db {
namespace tests {

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

}  // namespace tests
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_TESTS_TESTS_DB_SIMPLE_TEST_LOCKFREE_QUEUE_H_
