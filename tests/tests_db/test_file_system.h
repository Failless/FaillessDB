#ifndef FAILLESS_TESTS_TESTS_DB_TEST_FILE_SYSTEM_H
#define FAILLESS_TESTS_TESTS_DB_TEST_FILE_SYSTEM_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>

#include "llssdb/utils/task.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/task_worker.h"
#include "tests/tests_db/mocks.h"

namespace failless::db::tests {

using ::testing::_;
using ::testing::AtLeast;
using folder::FileSystem;
using common::enums::response_type;

common::utils::Data create_test_data() {
    size_t size = 3;
    std::vector<uint8_t> value = {1, 2, 3};
    std::string key = "test_key";
    return {0, size, value, &key};
}

TEST(FileSystem, Set) {
    FileSystem fs(kTestDbPath);

    /// Test value
    auto test_data = create_test_data();

    /// Empty arguments for Get()
    uint8_t* value_out = nullptr;
    size_t size_out = 0;

    fs.Set(test_data.key, const_cast<uint8_t *>(test_data.value.data()),test_data.size);
    EXPECT_EQ(fs.Get(test_data.key, value_out, size_out), response_type::OK);
    EXPECT_EQ( size_out, test_data.size);

    fs.EraseAll(kTestDbPath);
    EXPECT_EQ(fs.Get(test_data.key, value_out, size_out), response_type::NOT_FOUND);
}

TEST(FileSystem, Get) {
    FileSystem fs(kTestDbPath);

    /// Test value
    auto test_data = create_test_data();

    /// Empty arguments for Get()
    uint8_t* value_out = nullptr;
    size_t size_out = 0;

    /// Get() from empty db
    fs.EraseAll(kTestDbPath);
    EXPECT_EQ(fs.Get(test_data.key, value_out, size_out), response_type::NOT_FOUND);

    fs.Set(test_data.key, const_cast<uint8_t *>(test_data.value.data()), test_data.size);

    /// Get() wrong key
    std::string wrong_key = "wrong_key";
    EXPECT_EQ(fs.Get(wrong_key, value_out, size_out), response_type::NOT_FOUND);

    /// Actual Get()
    EXPECT_EQ(fs.Get(test_data.key, value_out, size_out), response_type::OK);
    EXPECT_EQ( size_out, test_data.size);

    fs.EraseAll(kTestDbPath);
}

TEST(FileSystem, Remove) {
    FileSystem fs(kTestDbPath);

    /// Test value
    auto test_data = create_test_data();

    /// Empty arguments for Get()
    uint8_t* value_out = nullptr;
    size_t size_out = 0;

    /// Remove() from empty db
    fs.EraseAll(kTestDbPath);
    EXPECT_EQ(fs.Remove(test_data.key), response_type::NOT_FOUND);

    fs.Set(test_data.key, const_cast<uint8_t *>(test_data.value.data()),test_data.size);

    /// Remove() wrong key
    std::string wrong_key = "wrong_key";
    EXPECT_EQ(fs.Remove(wrong_key), response_type::NOT_FOUND);

    /// Actual Remove()
    EXPECT_EQ(fs.Remove(test_data.key), response_type::OK);
    EXPECT_EQ(fs.Get(test_data.key, value_out, size_out), response_type::NOT_FOUND);

    fs.EraseAll(kTestDbPath);
}

TEST(FileSystem, Complex_Test) {
    FileSystem fs(kTestDbPath);

    /// Test value
    auto test_data = create_test_data();

    /// Empty arguments for Get()
    uint8_t* value_out = nullptr;
    size_t size_out = 0;

    // it's important to keep them together
    // so the db won't be filled with same pairs
    EXPECT_EQ(fs.Set(test_data.key, const_cast<uint8_t *>(test_data.value.data()), test_data.size), response_type::OK);
    EXPECT_EQ(fs.Get(test_data.key, value_out, size_out), response_type::OK);
    EXPECT_EQ( size_out, test_data.size);
    EXPECT_EQ(fs.Remove(test_data.key), response_type::OK);
    fs.EraseAll(kTestDbPath);
}

}


#endif // FAILLESS_TESTS_TESTS_DB_TEST_FILE_SYSTEM_H
