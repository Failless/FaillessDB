#ifndef FAILLESS_TEST_FILE_SYSTEM_H
#define FAILLESS_TEST_FILE_SYSTEM_H


#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

#include "llssdb/folder/task.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/task_worker.h"


namespace failless::db::tests {

using ::testing::_;
using ::testing::AtLeast;
using folder::FileSystem;
using std::string;

class MockFileSystem : public FileSystem {
public:
  explicit MockFileSystem(const string& db_path) : FileSystem(db_path) {};
  MOCK_METHOD1(Get, bool(string key));
  MOCK_METHOD2(Set, bool(string key, int8_t value));
  MOCK_METHOD1(GetRange, bool(string key));
  MOCK_METHOD1(Remove, bool(string key));
};

string test_db_path = "/storage/test_user/test_file";

TEST(FileSystem, Set_and_Get) {
    FileSystem fs(test_db_path);
    string key = "test_key";
    int8_t value = 32;

    EXPECT_EQ(fs.Set(key, value), true);
    EXPECT_EQ(fs.Get(key), true);
}

TEST(FileSystem, GetRange) {
    FileSystem fs(test_db_path);
    string key = "test_key";

//    EXPECT_CALL(fs, GetRange(key)).Times(1);
//    fs.GetRange(key);
}

TEST(FileSystem, Remove) {
    FileSystem fs(test_db_path);
    string key = "test_key";

//    EXPECT_CALL(fs, Remove(key)).Times(1);
//    fs.Remove(key);
}

}


#endif // FAILLESS_TEST_FILE_SYSTEM_H
