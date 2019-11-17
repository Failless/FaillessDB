#ifndef LLSSDB_TESTS_TEST_AUTH_H_
#define LLSSDB_TESTS_TEST_AUTH_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/auth/authorization.h"

using ::testing::_;
using ::testing::AtLeast;

class MockTaskWorker : public Authorization {
public:
//    explicit MockTaskWorker() : IAuthorization() {};
    MOCK_METHOD2(Reg, bool(std::string login, std::string pass));
    // MOCK_METHOD2(Login, bool(std::string login, std::string pass));
    // MOCK_METHOD2(Disconnect, bool(std::string login, std::string pass));
};

TEST(IAuthorization, Reg) {
    MockTaskWorker mockTaskWorker;
    std::string login = "login";
    std::string pass = "pass";
    EXPECT_CALL(mockTaskWorker, Reg(login, pass)).Times(AtLeast(1));
    EXPECT_EQ(mockTaskWorker.Test(login, pass), EXIT_SUCCESS);
}

#endif
