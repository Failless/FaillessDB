#ifndef LLSSDB_TESTS_TEST_AUTH_H_
#define LLSSDB_TESTS_TEST_AUTH_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/auth/authorization.h"

using ::testing::_;
using ::testing::AtLeast;

class MockIAuth : public IAuthorization {
public:
    // MOCK_METHOD1(CheckCollisions, bool(std::string login));
    MOCK_METHOD2(Registration, bool(std::string login, std::string pass));
    MOCK_METHOD2(SignIn, bool(std::string login, std::string pass));
};

TEST(IAuthorization, Registration) {
    MockIAuth iauth;
    std::string login = "login";
    std::string pass = "pass";
    EXPECT_CALL(iauth, Registration(login, pass)).Times(AtLeast(1));
    EXPECT_EQ(iauth.Test(login, pass), EXIT_SUCCESS);
}

class MockAuth : public Authorization {
public:
    MOCK_METHOD2(Registration, bool(std::string login, std::string pass));
    // MOCK_METHOD2(SignIn, bool(std::string login, std::string pass));
    MOCK_METHOD1(CheckCollisions, bool(std::string login));
};

TEST(Authorization, SignIn) {
    MockAuth auth;
    std::string login = "login";
    std::string pass = "pass";

    EXPECT_CALL(auth, CheckCollisions(login)).Times(AtLeast(1));
    EXPECT_EQ(auth.SignIn(login, pass), EXIT_SUCCESS);
}

#endif