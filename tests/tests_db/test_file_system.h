#ifndef FAILLESS_TEST_FILE_SYSTEM_H_
#define FAILLESS_TEST_FILE_SYSTEM_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/task_worker.h"
#include "llssdb/utils/task.h"

namespace failless::db::tests {

using folder::FileSystem;
using std::string;
using ::testing::_;
using ::testing::AtLeast;

class MockFileSystem : public FileSystem {
 public:
    explicit MockFileSystem(const string& db_path) : FileSystem(db_path){};
    MOCK_METHOD3(Get, bool(const string& key, uint8_t* value_out, size_t size_out));
    MOCK_METHOD3(Set, bool(const string& key, uint8_t* value_in, size_t size_in));
    //  MOCK_METHOD1(GetRange, bool(const string& key));
    MOCK_METHOD1(Remove, bool(const string& key));
};

// string test_db_path = "llssdb/CMakeFiles/llssdb.dir/storage/test_user";

TEST(FileSystem, Set_And_Get) {
    FileSystem fs(test_db_path);
    string key = "test_key";
    size_t size = 3;
    uint8_t value[size];
    for (size_t i = 0; i < size; ++i) {
        value[i] = i;
    }

    fs.Set(key, value, size);
    EXPECT_EQ(fs.Get(key, value), size);
    fs.EraseAll(test_db_path);
    EXPECT_EQ(fs.Get(key, value), 0);
}

TEST(FileSystem, Full_Functionality_Test) {
    FileSystem fs(test_db_path);
    string key = "test_key";
    size_t size = 3;
    uint8_t value[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value[iii] = iii;
    }

    // it's important to keep them together
    // so the db won't be filled with same pairs
    EXPECT_EQ(fs.Set(key, value, size), true);
    uint8_t *get_data = nullptr;
    EXPECT_EQ(fs.Get(key, get_data), size);
    EXPECT_EQ(fs.Remove(key), true);
    fs.EraseAll(test_db_path);
}

}  // namespace failless::db::tests

#endif  // FAILLESS_TEST_FILE_SYSTEM_H_