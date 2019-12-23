#ifndef FAILLESS_TESTS_TESTS_DB_TEST_FILE_SYSTEM_H
#define FAILLESS_TESTS_TESTS_DB_TEST_FILE_SYSTEM_H


#include "llssdb/folder/file_system.h"

#include <cstdlib>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include "llss3p/enums/operators.h"
#include "llssdb/folder/in_memory_data.h"
#include "tests/tests_db/mocks.h"

namespace failless::db::tests {

using ::testing::_;
using ::testing::AtLeast;
using folder::FileSystem;
using common::enums::response_type;

common::utils::Data prepare_test() {
    boost::filesystem::remove_all(kTestDbPath + "/1");
    boost::filesystem::create_directory(kTestDbPath + "/1");

    size_t size = 3;
    std::vector<uint8_t> value = {1, 2, 3};
    std::string key = "test_key";
    return {0, size, value, &key};
}

TEST(FileSystem, Set) {
    /// Test values
    auto test_data = prepare_test();
    common::utils::Data data_out;

    FileSystem fs(kTestDbPath + "/1");

    fs.Set(test_data);
    EXPECT_EQ(fs.Get(test_data.key, data_out.value, data_out.size), response_type::OK);
    EXPECT_EQ(data_out.size, test_data.size);

    fs.EraseAll();
    EXPECT_EQ(fs.Get(test_data.key, data_out.value, data_out.size), response_type::NOT_FOUND);
}

TEST(FileSystem, Get) {
    /// Test values
    auto test_data = prepare_test();
    common::utils::Data data_out;

    FileSystem fs(kTestDbPath + "/1");

    /// Get() from empty db
    fs.EraseAll();
    EXPECT_EQ(fs.Get(test_data.key, data_out.value, data_out.size), response_type::NOT_FOUND);

    fs.Set(test_data);

    /// Get() wrong key
    std::string wrong_key = "wrong_key";
    EXPECT_EQ(fs.Get(wrong_key, data_out.value, data_out.size), response_type::NOT_FOUND);

    /// Actual Get()
    EXPECT_EQ(fs.Get(test_data.key, data_out.value, data_out.size), response_type::OK);
    EXPECT_EQ(data_out.size, test_data.size);
}

TEST(FileSystem, Remove) {
    /// Test values
    auto test_data = prepare_test();
    common::utils::Data data_out;

    FileSystem fs(kTestDbPath + "/1");

    /// Remove() from empty db
    fs.EraseAll();
    EXPECT_EQ(fs.Remove(test_data.key), response_type::NOT_FOUND);

    fs.Set(test_data);

    /// Remove() wrong key
    std::string wrong_key = "wrong_key";
    EXPECT_EQ(fs.Remove(wrong_key), response_type::NOT_FOUND);

    /// Actual Remove()
    EXPECT_EQ(fs.Remove(test_data.key), response_type::OK);
    EXPECT_EQ(fs.Get(test_data.key, data_out.value, data_out.size), response_type::NOT_FOUND);
}

TEST(FileSystem, AmountOfKeys) {
    /// Test values
    auto test_data = prepare_test();
    common::utils::Data data_out;

    FileSystem fs(kTestDbPath + "/1");

    EXPECT_EQ(fs.AmountOfKeys(), 0);
    EXPECT_EQ(fs.Set(test_data), response_type::OK);
    EXPECT_EQ(fs.AmountOfKeys(), 1);
    EXPECT_EQ(fs.Set(test_data), response_type::OK);
    EXPECT_EQ(fs.AmountOfKeys(), 2);
    // 2 because rocksDB allows multiple values with the same key
    // we don't so this behaviour is handled by TaskWorker
    EXPECT_EQ(fs.Remove(test_data.key), response_type::OK);
    fs.EraseAll();
    EXPECT_EQ(fs.AmountOfKeys(), 0);
}

TEST(FileSystem, LoadCache) {
    /// Test values
    auto test_data = prepare_test();
    std::unordered_map<std::string, folder::InMemoryData> local_storage_;

    FileSystem fs(kTestDbPath + "/1");

    EXPECT_EQ(fs.Set(test_data), response_type::OK);
    test_data.key = "test_key1";
    test_data.size = 4;
    test_data.value = {1, 2, 3, 4};
    EXPECT_EQ(fs.Set(test_data), response_type::OK);
    test_data.key = "test_key2";
    test_data.size = 5;
    test_data.value = {1, 2, 3, 4, 5};
    EXPECT_EQ(fs.Set(test_data), response_type::OK);

    long cur_bytes = 0;
    fs.LoadCache(local_storage_, 1024*1024*4, cur_bytes);
    EXPECT_EQ(local_storage_.size(), 3);
    EXPECT_EQ(local_storage_.at("test_key").value.size(), 3);
    EXPECT_EQ(local_storage_.at("test_key1").value.size(), 4);
    EXPECT_EQ(local_storage_.at("test_key2").value.size(), 5);
}

TEST(FileSystem, Complex_Test) {
    /// Test values
    auto test_data = prepare_test();
    common::utils::Data data_out;

    FileSystem fs(kTestDbPath + "/1");

    EXPECT_EQ(fs.Set(test_data), response_type::OK);
    EXPECT_EQ(fs.Get(test_data.key, data_out.value, data_out.size), response_type::OK);
    EXPECT_EQ(data_out.size, test_data.size);
    EXPECT_EQ(fs.Remove(test_data.key), response_type::OK);
}

}


#endif // FAILLESS_TESTS_TESTS_DB_TEST_FILE_SYSTEM_H
