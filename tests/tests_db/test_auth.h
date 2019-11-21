#ifndef LLSSDB_TESTS_TEST_AUTH_H_
#define LLSSDB_TESTS_TEST_AUTH_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/auth/authorization.h"

using ::testing::_;
using ::testing::AtLeast;


class MockAuth : public Authorization {
public:
    std::string login;
    std::string pass;
    MOCK_METHOD1(CheckCollisions_, bool(std::string login));
    MOCK_METHOD2(Hasher_, std::string(std::string login, std::string pass));
    MOCK_METHOD1(IsAuth_, bool(std::string login));
    MockAuth() {
        login = "login";
        pass = "pass";
    }
};

TEST(Authorization, SignIn) {
    MockAuth auth;
    EXPECT_CALL(auth, IsAuth_(auth.login)).Times(AtLeast(1));
    EXPECT_EQ(auth.SignIn(auth.login, auth.pass), true);
}

TEST(Hasher, SignIn) {
    MockAuth auth;
    EXPECT_CALL(auth, Hasher_(auth.login, auth.pass)).Times(AtLeast(1));
    EXPECT_EQ(auth.SignIn(auth.login, auth.pass), true);
}

TEST(Registration, Registration) {
    MockAuth auth;
    EXPECT_CALL(auth, CheckCollisions_(auth.login)).Times(AtLeast(1));
    EXPECT_EQ(auth.Registration(auth.login, auth.pass), true);
}

TEST(Hasher, Registration) {
    MockAuth auth;
    EXPECT_CALL(auth, Hasher_(auth.login, auth.pass)).Times(AtLeast(1));
    EXPECT_EQ(auth.Registration(auth.login, auth.pass), true);
}


class MockIAuth : public IAuthorization {
public:
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

#endif