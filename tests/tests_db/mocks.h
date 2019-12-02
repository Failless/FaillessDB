#ifndef FAILLESS_MOCKS_H
#define FAILLESS_MOCKS_H

#include <gmock/gmock.h>
#include <string>

///////////////////////////////////////////////////////////////////////////////////
/// README:                                                                     ///
/// If you build this project via CLion:                                        ///
///      1. Choose first path                                                   ///
///      2. Comment second one                                                  ///
///      3. Create next empty folders manually:                                 ///
///          cmake-build-debug/llssdb/CMakeFiles/llssdb.dir/storage             ///
///          cmake-build-debug/llssdb/CMakeFiles/llssdb.dir/storage/test_user   ///
///                                                                             ///
/// If you build this project via "cmake . && make":                            ///
///      1. Choose second path                                                  ///
///      2. Comment first one                                                   ///
///////////////////////////////////////////////////////////////////////////////////

const std::string test_db_path = "llssdb/CMakeFiles/llssdb.dir/storage/test_user";
//const std::string test_db_path = "./llssdb/storage/test_user";

namespace failless::db::tests {

using ::testing::_;

 class MockTaskWorker : public folder::TaskWorker {
 public:
    explicit MockTaskWorker(const std::string &db_path = test_db_path) : folder::TaskWorker(db_path){};
    MOCK_METHOD1(Set, bool(const utils::Task &task_in));
    MOCK_METHOD1(Read, bool(const utils::Task &task_in));
    MOCK_METHOD1(Update, bool(const utils::Task &task_in));
    MOCK_METHOD1(Delete, bool(const utils::Task &task_in));
    MOCK_METHOD0(LoadInMemory, void());
    MOCK_METHOD0(UnloadFromMemory, void());
};

class MockFileSystem : public folder::FileSystem {
public:
  explicit MockFileSystem(const std::string& db_path) : folder::FileSystem(db_path) {};
  MOCK_METHOD3(Get, bool(const std::string &key, std::shared_ptr<int8_t>& value_out, size_t& size_out));
  MOCK_METHOD3(Set, bool(const std::string &key, const std::shared_ptr<int8_t>& value_in, size_t size_in));
//  MOCK_METHOD1(GetRange, bool(const std::string& key));
  MOCK_METHOD1(Remove, bool(const std::string& key));
};

}

#endif  // FAILLESS_MOCKS_H
