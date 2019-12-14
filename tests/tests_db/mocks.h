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

class TestTaskWorker : public folder::TaskWorker {
public:
    explicit TestTaskWorker(common::utils::Queue<std::shared_ptr<network::Connection>>& queue,
                            const std::string& kTestDbPath)
            : folder::TaskWorker(queue, kTestDbPath) {};
protected:
    void SendAnswer(std::shared_ptr<network::Connection>& conn,
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

    MOCK_METHOD1(Set, common::enums::response_type(common::utils::Data& data));
    MOCK_METHOD1(Read, common::enums::response_type(common::utils::Data& data));
    MOCK_METHOD1(Update, common::enums::response_type(common::utils::Data& data));
    MOCK_METHOD1(Delete, common::enums::response_type(common::utils::Data& data));
    MOCK_METHOD0(LoadInMemory, void());
    MOCK_METHOD0(UnloadFromMemory, void());
};

class MockFileSystem : public folder::FileSystem {
public:
    explicit MockFileSystem(const std::string &db_path) : folder::FileSystem(db_path) {};

    MOCK_METHOD3(Get, common::enums::response_type(
            const std::string &key,
            uint8_t *value_out,
            size_t size_out));

    MOCK_METHOD3(Set, common::enums::response_type(
            const std::string &key,
            uint8_t *value_in,
            size_t size_in));

    MOCK_METHOD1(Remove, common::enums::response_type(
            const std::string &key));
};
}


#endif  // FAILLESS_TESTS_TESTS_DB_MOCKS_H
