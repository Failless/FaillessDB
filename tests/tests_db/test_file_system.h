#ifndef FAILLESS_TEST_FILE_SYSTEM_H
#define FAILLESS_TEST_FILE_SYSTEM_H


#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

#include "llssdb/common/task.h"
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
  MOCK_METHOD1(Get, bool(const string& key));
  MOCK_METHOD2(Set, bool(const string& key, int8_t value));
  MOCK_METHOD1(GetRange, bool(const string& key));
  MOCK_METHOD1(Remove, bool(const string& key));
};

//string test_db_path = "llssdb/CMakeFiles/llssdb.dir/storage/test_user";

TEST(FileSystem, Set_And_Get) {
    FileSystem fs(test_db_path);
    string key = "test_key";
    int8_t value = 100;

    fs.Set(key, value);
    EXPECT_EQ(fs.Get(key), true);
    fs.EraseAll(test_db_path);
    EXPECT_EQ(fs.Get(key), false);
}

TEST(FileSystem, Full_Functionality_Test) {
    FileSystem fs(test_db_path);
    string key = "test_key";
    int8_t value = 100;

    // it's important to keep them together
    // so the db won't be filled with same pairs
    EXPECT_EQ(fs.Set(key, value), true);
    EXPECT_EQ(fs.Get(key), true);
    EXPECT_EQ(fs.Remove(key), true);
}


}


#endif // FAILLESS_TEST_FILE_SYSTEM_H
