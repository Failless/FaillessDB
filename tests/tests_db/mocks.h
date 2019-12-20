#ifndef FAILLESS_TESTS_TESTS_DB_MOCKS_H
#define FAILLESS_TESTS_TESTS_DB_MOCKS_H


#include "llssdb/folder/file_system.h"
#include "llssdb/folder/task_worker.h"

#include <cstdlib>
#include <memory>
#include <string>

#include <boost/filesystem.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llss3p/enums/operators.h"
#include "llssdb/network/transfer/hookup.h"

std::string kTestDbPath{};

namespace failless::db::tests {
using ::testing::_;
using namespace boost::filesystem;

void set_test_db_path() {
    /// DEBUG
    if ( exists("llssdb/CMakeFiles/llssdb.dir") ) {
        if ( exists("llssdb/CMakeFiles/llssdb.dir/storage") ) {
            if ( exists("llssdb/CMakeFiles/llssdb.dir/storage/test_user") ) {
                detail::remove_all("llssdb/CMakeFiles/llssdb.dir/storage/test_user/");
            }
            detail::create_directory("llssdb/CMakeFiles/llssdb.dir/storage/test_user");
        } else {
            detail::create_directories("llssdb/CMakeFiles/llssdb.dir/storage/test_user");
        }
        kTestDbPath = "llssdb/CMakeFiles/llssdb.dir/storage/test_user";
    }
    /// DEPLOY
    else if ( exists("./llssdb/storage/") ) {
        if ( exists("./llssdb/storage/test_user") ) {
            detail::remove_all("./llssdb/storage/test_user/");
        }
        kTestDbPath = "./llssdb/storage/test_user";
    }
}

class TestTaskWorker : public folder::TaskWorker {
public:
    explicit TestTaskWorker(common::utils::Queue<std::shared_ptr<network::Connection>>& queue,
                            const std::string& kTestDbPath)
            : folder::TaskWorker(queue, kTestDbPath) {};
protected:
    void SendAnswer_(std::shared_ptr<network::Connection>& conn,
                    common::enums::response_type result,
                    bool read) override {
        // Prepare return_packet
        conn->GetPacket()->ret_value = static_cast<int>(result);
        if ( !read ) {
            conn->GetPacket()->data.value = {};
        }
//        conn->SendData(*(conn->GetPacket()));
    }
};

class MockTaskWorker : public TestTaskWorker {
public:
    explicit MockTaskWorker(common::utils::Queue<std::shared_ptr<network::Connection>>& queue,
                            const std::string& kTestDbPath)
            : TestTaskWorker(queue, kTestDbPath) {};

    MOCK_METHOD1(Set_, common::enums::response_type(common::utils::Data& data));
    MOCK_METHOD1(Read_, common::enums::response_type(common::utils::Data& data));
    MOCK_METHOD1(Update_, common::enums::response_type(common::utils::Data& data));
    MOCK_METHOD1(Delete_, common::enums::response_type(common::utils::Data& data));
    MOCK_METHOD0(LoadInMemory_, void());
    MOCK_METHOD0(UnloadFromMemory_, void());
};

class MockFileSystem : public folder::FileSystem {
public:
    explicit MockFileSystem(const std::string &folder_path) : folder::FileSystem(folder_path) {};

    MOCK_METHOD3(Get, common::enums::response_type(
            const std::string &key,
            uint8_t *value_out,
            size_t& size_out));

    MOCK_METHOD3(Set, common::enums::response_type(
            const std::string &key,
            uint8_t *value_in,
            size_t size_in));

    MOCK_METHOD1(Remove, common::enums::response_type(
            const std::string &key));
};
}


#endif  // FAILLESS_TESTS_TESTS_DB_MOCKS_H
