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

std::string kTestDbPath("/tmp/failless/storage/test_user");
std::string kStoragePath("/tmp/failless/storage");

namespace failless::db::tests {
using ::testing::_;

void set_test_db_path() {
    /// DEBUG
    boost::filesystem::create_directory("/tmp/failless");
    boost::filesystem::create_directory("/tmp/failless/storage");
    boost::filesystem::remove_all("/tmp/failless/storage");
    boost::filesystem::create_directory("/tmp/failless/storage");
    boost::filesystem::create_directory("/tmp/failless/storage/test_user");
}

class TestTaskWorker : public folder::TaskWorker {
public:
    explicit TestTaskWorker(common::utils::Queue<std::shared_ptr<network::Connection>>& queue,
                            const std::string& kTestDbPath,
                            bool do_backup = false)
            : folder::TaskWorker(queue, kTestDbPath, do_backup) {};
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
                            const std::string& kTestDbPath,
                            bool do_backup = false)
            : TestTaskWorker(queue, kTestDbPath, do_backup) {};

    MOCK_METHOD1(Set_, common::enums::response_type(common::utils::Data& data));
    MOCK_METHOD1(Read_, common::enums::response_type(common::utils::Data& data));
    MOCK_METHOD1(Delete_, common::enums::response_type(common::utils::Data& data));
    MOCK_METHOD0(LoadCache_, void());
    MOCK_METHOD0(ClearCache_, void());
};

class MockFileSystem : public folder::FileSystem {
public:
    explicit MockFileSystem(const std::string &folder_path, bool do_backup)
      : folder::FileSystem(folder_path, do_backup) {};

    MOCK_METHOD3(Get, common::enums::response_type(
            const std::string &key,
            std::vector<uint8_t>& value_out,
            size_t& size_out));

    MOCK_METHOD1(Set, common::enums::response_type(
            common::utils::Data& data));

    MOCK_METHOD1(Remove, common::enums::response_type(
            const std::string &key));
};
}


#endif  // FAILLESS_TESTS_TESTS_DB_MOCKS_H
