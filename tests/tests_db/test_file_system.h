#ifndef TESTS_TESTS_DB_TEST_FILE_SYSTEM_H
#define TESTS_TESTS_DB_TEST_FILE_SYSTEM_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

#include "llssdb/common/task.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/task_worker.h"
#include "tests/tests_db/mocks.h"

namespace failless::db::tests {

using ::testing::_;
using ::testing::AtLeast;
using folder::FileSystem;

TEST(FileSystem, Set_And_Get) {
    FileSystem fs(test_db_path);
    std::string key = "test_key";
    size_t size = 3;
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});

    fs.Set(key, value, size);
    EXPECT_EQ(fs.Get(key, value, size), true);
    fs.EraseAll(test_db_path);
    EXPECT_EQ(fs.Get(key, value, size), false);
}

TEST(FileSystem, Full_Functionality_Test) {
    FileSystem fs(test_db_path);
    std::string key = "test_key";
    size_t size = 3;
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});

    // it's important to keep them together
    // so the db won't be filled with same pairs
    EXPECT_EQ(fs.Set(key, value, size), true);
    EXPECT_EQ(fs.Get(key, value, size), true);
    EXPECT_EQ(fs.Remove(key), true);
    fs.EraseAll(test_db_path);
}


}


#endif // TESTS_TESTS_DB_TEST_FILE_SYSTEM_H
