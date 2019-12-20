#ifndef FAILLESS_TESTS_TESTS_DB_TEST_FILE_SYSTEM_H
#define FAILLESS_TESTS_TESTS_DB_TEST_FILE_SYSTEM_H


#include "llssdb/folder/file_system.h"

#include <cstdlib>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include "llss3p/enums/operators.h"
#include "tests/tests_db/mocks.h"

namespace failless::db::tests {

using ::testing::_;
using ::testing::AtLeast;
using folder::FileSystem;
using common::enums::response_type;

common::utils::Data prepare_test() {
    boost::filesystem::create_directory(kTestDbPath + "/0");

    size_t size = 3;
    std::vector<uint8_t> value = {1, 2, 3};
    std::string key = "test_key";
    return {0, size, value, &key};
}

TEST(FileSystem, Set) {
    /// Test values
    auto test_data = prepare_test();

    FileSystem fs(kTestDbPath + "/0");

    /// Empty arguments for Get()
    uint8_t* value_out = nullptr;
    size_t size_out = 0;

    fs.Set(test_data.key, const_cast<uint8_t *>(test_data.value.data()),test_data.size);
    EXPECT_EQ(fs.Get(test_data.key, value_out, size_out), response_type::OK);
    EXPECT_EQ( size_out, test_data.size);

    fs.EraseAll();
    EXPECT_EQ(fs.Get(test_data.key, value_out, size_out), response_type::NOT_FOUND);
    boost::filesystem::remove_all(kTestDbPath + "/0");
}

TEST(FileSystem, Get) {
    /// Test values
    auto test_data = prepare_test();

    FileSystem fs(kTestDbPath + "/0");

    /// Empty arguments for Get()
    uint8_t* value_out = nullptr;
    size_t size_out = 0;

    /// Get() from empty db
    fs.EraseAll();
    EXPECT_EQ(fs.Get(test_data.key, value_out, size_out), response_type::NOT_FOUND);

    fs.Set(test_data.key, const_cast<uint8_t *>(test_data.value.data()), test_data.size);

    /// Get() wrong key
    std::string wrong_key = "wrong_key";
    EXPECT_EQ(fs.Get(wrong_key, value_out, size_out), response_type::NOT_FOUND);

    /// Actual Get()
    EXPECT_EQ(fs.Get(test_data.key, value_out, size_out), response_type::OK);
    EXPECT_EQ( size_out, test_data.size);

    fs.EraseAll();
    boost::filesystem::remove_all(kTestDbPath + "/0");
}

TEST(FileSystem, Remove) {
    /// Test values
    auto test_data = prepare_test();
    FileSystem fs(kTestDbPath + "/0");

    /// Empty arguments for Get()
    uint8_t* value_out = nullptr;
    size_t size_out = 0;

    /// Remove() from empty db
    fs.EraseAll();
    EXPECT_EQ(fs.Remove(test_data.key), response_type::NOT_FOUND);

    fs.Set(test_data.key, const_cast<uint8_t *>(test_data.value.data()),test_data.size);

    /// Remove() wrong key
    std::string wrong_key = "wrong_key";
    EXPECT_EQ(fs.Remove(wrong_key), response_type::NOT_FOUND);

    /// Actual Remove()
    EXPECT_EQ(fs.Remove(test_data.key), response_type::OK);
    EXPECT_EQ(fs.Get(test_data.key, value_out, size_out), response_type::NOT_FOUND);

    fs.EraseAll();
    boost::filesystem::remove_all(kTestDbPath + "/0");
}

TEST(FileSystem, Complex_Test) {
    /// Test values
    auto test_data = prepare_test();

    FileSystem fs(kTestDbPath + "/0");

    /// Empty arguments for Get()
    uint8_t* value_out = nullptr;
    size_t size_out = 0;

    // it's important to keep them together
    // so the db won't be filled with same pairs
    EXPECT_EQ(fs.Set(test_data.key, const_cast<uint8_t *>(test_data.value.data()), test_data.size), response_type::OK);
    EXPECT_EQ(fs.Get(test_data.key, value_out, size_out), response_type::OK);
    EXPECT_EQ( size_out, test_data.size);
    EXPECT_EQ(fs.Remove(test_data.key), response_type::OK);
    fs.EraseAll();
    boost::filesystem::remove_all(kTestDbPath + "/0");
}

}


#endif // FAILLESS_TESTS_TESTS_DB_TEST_FILE_SYSTEM_H
