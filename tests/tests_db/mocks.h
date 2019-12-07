#ifndef FAILLESS_TESTS_TESTS_DB_MOCKS_H
#define FAILLESS_TESTS_TESTS_DB_MOCKS_H

#include <gmock/gmock.h>
#include <string>

std::string kTestDbPath;

namespace failless::db::tests {
using ::testing::_;

void set_test_db_path() {
    if ( boost::filesystem::exists("llssdb/CMakeFiles/llssdb.dir") ) {
        if ( !boost::filesystem::exists("llssdb/CMakeFiles/llssdb.dir/storage") ) {
            boost::filesystem::detail::create_directories("llssdb/CMakeFiles/llssdb.dir/storage/test_user");
        } else {
            if ( boost::filesystem::exists("llssdb/CMakeFiles/llssdb.dir/storage/test_user") ) {
                boost::filesystem::detail::remove_all("llssdb/CMakeFiles/llssdb.dir/storage/test_user/");
            }
            boost::filesystem::detail::create_directory("llssdb/CMakeFiles/llssdb.dir/storage/test_user");
        }
        kTestDbPath = "llssdb/CMakeFiles/llssdb.dir/storage/test_user";
    } else if ( boost::filesystem::exists("./llssdb/storage/") ) {
        if ( boost::filesystem::exists("./llssdb/storage/test_user") ) {
            boost::filesystem::detail::remove_all("./llssdb/storage/test_user/");
        }
        boost::filesystem::detail::create_directory("./llssdb/storage/test_user");
        kTestDbPath = "./llssdb/storage/test_user";
    }
}

class MockTaskWorker : public folder::TaskWorker {
public:
    explicit MockTaskWorker(common::utils::Queue<std::shared_ptr<network::Connection>>& queue,
                            const std::string& kTestDbPath)
            : folder::TaskWorker(queue, kTestDbPath) {};

    MOCK_METHOD1(Set, bool(common::utils::Data& data));
    MOCK_METHOD1(Read, bool(common::utils::Data& data));
    MOCK_METHOD1(Update, bool(common::utils::Data& data));
    MOCK_METHOD1(Delete, bool(common::utils::Data& data));
    MOCK_METHOD0(LoadInMemory, void());
    MOCK_METHOD0(UnloadFromMemory, void());
};

class MockFileSystem : public folder::FileSystem {
public:
    explicit MockFileSystem(const std::string &db_path) : folder::FileSystem(db_path) {};

    MOCK_METHOD2(Get, size_t(
            const std::string &key,
            uint8_t *value_out));

    MOCK_METHOD3(Set, bool(
            const std::string &key,
            uint8_t *value_in,
            size_t size_in));

    MOCK_METHOD1(Remove, bool(
            const std::string &key));
};
}

#endif  // FAILLESS_TESTS_TESTS_DB_MOCKS_H
