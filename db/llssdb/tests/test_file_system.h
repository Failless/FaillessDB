#ifndef FAILLESS_TEST_FILE_SYSTEM_H
#define FAILLESS_TEST_FILE_SYSTEM_H


#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/folder/task.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/task_worker.h"


namespace failless::db::tests {

using ::testing::_;
using ::testing::AtLeast;

class MockFileSystem : public FileSystem {
public:
  MockFileSystem() : FileSystem() {};
  MOCK_METHOD1(Get, bool(int key));
  MOCK_METHOD2(Set, bool(int key, u_int8_t data));
  MOCK_METHOD1(GetRange, bool(int key));
  MOCK_METHOD1(Remove, bool(int key));
};

TEST(FileSystem, Get) {
    MockFileSystem mockFileSystem;
    int key = 0;

    EXPECT_CALL(mockFileSystem, Get(key)).Times(1);
    mockFileSystem.Get(key);
}

TEST(FileSystem, Set) {
    MockFileSystem mockFileSystem;
    int key = 0;
    u_int8_t data = 0;

    EXPECT_CALL(mockFileSystem, Set(key, data)).Times(1);
    mockFileSystem.Set(key, data);
}

TEST(FileSystem, GetRange) {
    MockFileSystem mockFileSystem;
    int key = 0;

    EXPECT_CALL(mockFileSystem, GetRange(key)).Times(1);
    mockFileSystem.GetRange(key);
}

TEST(FileSystem, Remove) {
    MockFileSystem mockFileSystem;
    int key = 0;

    EXPECT_CALL(mockFileSystem, Remove(key)).Times(1);
    mockFileSystem.Remove(key);
}

}


#endif // FAILLESS_TEST_FILE_SYSTEM_H
