#ifndef TESTS_TESTS_DB_TEST_FILE_SYSTEM_H
#define TESTS_TESTS_DB_TEST_FILE_SYSTEM_H

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
/*
TEST(FileSystem, Set) {
    FileSystem fs(kTestDbPath);

    /// Test values
    std::string key = "test_key";
    size_t size = 3;
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});

    /// Empty arguments for Get()
    size_t size_out = 0;
    std::shared_ptr<int8_t> value_out;

<<<<<<< HEAD
    fs.Set(key, value, size);
    EXPECT_TRUE(fs.Get(key, value_out, size_out));
    EXPECT_EQ(size, size_out);
//    for ( size_t iii = 0; iii < size || iii < size_out; ++iii ) {
//        EXPECT_EQ(value.get()[iii], value_out.get()[iii]);
//    } // TODO(EgorBedov): values are the same but they're written differently (1 != x/1)
=======
class MockFileSystem : public FileSystem {
 public:
    explicit MockFileSystem(const string& db_path) : FileSystem(db_path){};
    MOCK_METHOD3(Get, bool(const string& key, uint8_t* value_out, size_t size_out));
    MOCK_METHOD3(Set, bool(const string& key, uint8_t* value_in, size_t size_in));
    //  MOCK_METHOD1(GetRange, bool(const string& key));
    MOCK_METHOD1(Remove, bool(const string& key));
};
>>>>>>> feature/network

    fs.EraseAll(kTestDbPath);
    EXPECT_FALSE(fs.Get(key, value_out, size_out));
}

TEST(FileSystem, Get) {
    FileSystem fs(kTestDbPath);

    /// Test values
    std::string key = "test_key";
    size_t size = 3;
<<<<<<< HEAD
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});

    /// Empty arguments for Get()
    size_t size_out = 0;
    std::shared_ptr<int8_t> value_out;

    /// Get() from empty db
    fs.EraseAll(kTestDbPath);
    EXPECT_FALSE(fs.Get(key, value_out, size_out));

    fs.Set(key, value, size);

    /// Get() wrong key
    std::string wrong_key = "wrong_key";
    EXPECT_FALSE(fs.Get(wrong_key, value_out, size_out));

    /// Actual Get()
    EXPECT_TRUE(fs.Get(key, value_out, size_out));

    fs.EraseAll(kTestDbPath);
}

TEST(FileSystem, Remove) {
    FileSystem fs(kTestDbPath);

    /// Test values
    std::string key = "test_key";
    size_t size = 3;
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});

    /// Empty arguments for Get()
    size_t size_out = 0;
    std::shared_ptr<int8_t> value_out;

    /// Remove() from empty db
    fs.EraseAll(kTestDbPath);
    EXPECT_FALSE(fs.Remove(key));

    fs.Set(key, value, size);

    /// Remove() wrong key
    std::string wrong_key = "wrong_key";
    EXPECT_FALSE(fs.Remove(wrong_key));

    /// Actual Remove()
    EXPECT_TRUE(fs.Remove(key));
    EXPECT_FALSE(fs.Get(key, value_out, size_out));

    fs.EraseAll(kTestDbPath);
=======
    uint8_t value[size];
    for (size_t i = 0; i < size; ++i) {
        value[i] = i;
    }

    fs.Set(key, value, size);
    EXPECT_EQ(fs.Get(key, value), size);
    fs.EraseAll(kTestDbPath);
    EXPECT_EQ(fs.Get(key, value), 0);
>>>>>>> feature/network
}

TEST(FileSystem, Full_Functionality_Test) {
    FileSystem fs(kTestDbPath);

    /// Test values
    std::string key = "test_key";
    size_t size = 3;
<<<<<<< HEAD
    std::shared_ptr<int8_t> value(new int8_t[size] {1, 2, 3});
=======
    uint8_t value[size];
    for (size_t iii = 0; iii < size; ++iii) {
        value[iii] = iii;
    }
>>>>>>> feature/network

    // it's important to keep them together
    // so the db won't be filled with same pairs
    EXPECT_EQ(fs.Set(key, value, size), true);
    uint8_t *get_data = nullptr;
    EXPECT_EQ(fs.Get(key, get_data), size);
    EXPECT_EQ(fs.Remove(key), true);
    fs.EraseAll(kTestDbPath);
}

*/

}


#endif // TESTS_TESTS_DB_TEST_FILE_SYSTEM_H
