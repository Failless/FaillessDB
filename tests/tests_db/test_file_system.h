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

common::utils::Data create_test_data() {
    size_t size = 3;
    std::vector<uint8_t> value = {1, 2, 3};
    auto key = new std::string("test_key");
    return {0, size, value, key};
}

TEST(FileSystem, Set) {
    FileSystem fs(kTestDbPath);

    /// Test value
    auto test_data = create_test_data();

    /// Empty arguments for Get()
    uint8_t* value_out = nullptr;

    fs.Set(test_data.key, const_cast<uint8_t *>(test_data.value.data()),test_data.size);
    EXPECT_EQ(test_data.size, fs.Get(test_data.key, value_out));
//    for ( size_t iii = 0; iii < size || iii < size_out; ++iii ) {
//        EXPECT_EQ(value.get()[iii], value_out.get()[iii]);
//    } // TODO(EgorBedov): values are the same but they're written differently (1 != x/1)

    fs.EraseAll(kTestDbPath);
    EXPECT_EQ(fs.Get(test_data.key, value_out), 0);
}

TEST(FileSystem, Get) {
    FileSystem fs(kTestDbPath);

    /// Test value
    auto test_data = create_test_data();

    /// Empty arguments for Get()
    uint8_t* value_out = nullptr;

    /// Get() from empty db
    fs.EraseAll(kTestDbPath);
    EXPECT_EQ(fs.Get(test_data.key, value_out), 0);

    fs.Set(test_data.key, const_cast<uint8_t *>(test_data.value.data()), test_data.size);

    /// Get() wrong key
    std::string wrong_key = "wrong_key";
    EXPECT_EQ(fs.Get(wrong_key, value_out), 0);

    /// Actual Get()
    EXPECT_EQ(test_data.size, fs.Get(test_data.key, value_out));

    fs.EraseAll(kTestDbPath);
}

TEST(FileSystem, Remove) {
    FileSystem fs(kTestDbPath);

    /// Test value
    auto test_data = create_test_data();

    /// Empty arguments for Get()
    uint8_t* value_out = nullptr;

    /// Remove() from empty db
    fs.EraseAll(kTestDbPath);
    EXPECT_FALSE(fs.Remove(test_data.key));

    fs.Set(test_data.key, const_cast<uint8_t *>(test_data.value.data()),test_data.size);

    /// Remove() wrong key
    std::string wrong_key = "wrong_key";
    EXPECT_FALSE(fs.Remove(wrong_key));

    /// Actual Remove()
    EXPECT_TRUE(fs.Remove(test_data.key));
    EXPECT_EQ(fs.Get(test_data.key, value_out), 0);

    fs.EraseAll(kTestDbPath);
}

TEST(FileSystem, Complex_Test) {
    FileSystem fs(kTestDbPath);

    /// Test value
    auto test_data = create_test_data();

    // it's important to keep them together
    // so the db won't be filled with same pairs
    EXPECT_EQ(fs.Set(test_data.key, const_cast<uint8_t *>(test_data.value.data()), test_data.size), true);
    EXPECT_EQ(fs.Get(test_data.key, nullptr), test_data.size);
    EXPECT_EQ(fs.Remove(test_data.key), true);
    fs.EraseAll(kTestDbPath);
}


}


#endif // FAILLESS_TESTS_TESTS_DB_TEST_FILE_SYSTEM_H
