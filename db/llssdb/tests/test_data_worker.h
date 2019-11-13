#ifndef FAILLESS_TEST_DATA_WORKER_H
#define FAILLESS_TEST_DATA_WORKER_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <set>
#include <string>

#include "llssdb/folder/data_worker.h"
#include "llssdb/folder/task_worker.h"

namespace failless::db::tests {

using ::testing::_;
using ::testing::AtLeast;

class MockDataWorker : public IDataWorker {
public:
    explicit MockDataWorker() : IDataWorker() {};
    MOCK_METHOD0(Create, bool());
    MOCK_METHOD0(Read, bool());
    MOCK_METHOD0(Update, bool());
    MOCK_METHOD0(Delete, bool());
    MOCK_METHOD0(SetDirectory, std::string());
    MOCK_METHOD0(SetLength, int());
//    MOCK_METHOD0(SetDataSet, std::set<int, std::string>());
};

TEST(IDataWorker, Create) {
    MockDataWorker mockDataWorker;
    EXPECT_CALL(mockDataWorker, SetDirectory()).Times(AtLeast(1));
    EXPECT_CALL(mockDataWorker, SetLength()).Times(AtLeast(1));
//    EXPECT_CALL(mockDataWorker, SetDataSet()).Times(AtLeast(1));

    TaskWorker tw;
}

} // namespace failless

#endif //FAILLESS_TEST_DATA_WORKER_H
