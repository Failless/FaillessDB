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
  MOCK_METHOD1(Get, u_int8_t(int key));
  MOCK_METHOD2(Set, bool(int key, u_int8_t data));
  MOCK_METHOD1(GetRange, u_int8_t(int key));
  MOCK_METHOD1(Remove, bool(int key));
};

}


#endif // FAILLESS_TEST_FILE_SYSTEM_H
