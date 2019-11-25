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
    MOCK_METHOD2(Hasher_, unsigned char *(std::string login, std::string pass));
    MockAuth() {
        login = "login";
        pass = "pass";
    }
};


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

TEST(Hasher, RemoveUser) {
    MockAuth auth;
    EXPECT_CALL(auth, Hasher_(auth.login, auth.pass)).Times(AtLeast(1));
    EXPECT_EQ(auth.RemoveUser(auth.login, auth.pass), false);
}


#endif