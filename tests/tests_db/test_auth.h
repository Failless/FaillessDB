#ifndef LLSSDB_TESTS_TEST_AUTH_H_
#define LLSSDB_TESTS_TEST_AUTH_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/auth/authorization.h"

using ::testing::_;
using ::testing::AtLeast;

class MockTaskWorker : public IAuthorization {
public:
    MOCK_METHOD1(CheckCollisions, bool(std::string login));
    MOCK_METHOD2(Registration, bool(std::string login, std::string pass));
    MOCK_METHOD2(SignIn, bool(std::string login, std::string pass));
};

TEST(IAuthorization, Registration) {
    MockTaskWorker mockTaskWorker;
    std::string login = "login";
    std::string pass = "pass";
    EXPECT_CALL(mockTaskWorker, Registration(login, pass)).Times(AtLeast(1));
    EXPECT_EQ(mockTaskWorker.Test(login, pass), EXIT_SUCCESS);
}

TEST(IAuthorization, SignIn) {
    MockTaskWorker mockTaskWorker;
    std::string login = "login";
    std::string pass = "pass";
    EXPECT_CALL(mockTaskWorker, SignIn(login, pass)).Times(AtLeast(1));
    EXPECT_EQ(mockTaskWorker.CheckCollisions(login), EXIT_SUCCESS);
}

#endif