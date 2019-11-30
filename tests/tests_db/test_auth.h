#ifndef LLSSDB_TESTS_TESTS_DB_AUTH_H_
#define LLSSDB_TESTS_TESTS_DB_AUTH_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "llssdb/auth/authorization.h"

using ::testing::_;
using ::testing::AtLeast;

class MockAuth : public Authorization {
 public:
    std::string login;
    std::string pass;
    unsigned char pass_hash[32];
    MOCK_METHOD1(CheckCollisions_, bool(std::string login));
    MOCK_METHOD3(Hasher_, unsigned char *(std::string login, std::string pass, unsigned char *md));
    MockAuth() {
        login = "login";
        pass = "pass";
        // pass_hash = "pass_hash";
    }
};

TEST(Registration, Registration) {
    MockAuth auth;
    EXPECT_CALL(auth, CheckCollisions_(auth.login)).Times(AtLeast(1));
    EXPECT_EQ(auth.Registration(auth.login, auth.pass), true);
}

TEST(Hasher, Registration) {
    MockAuth auth;
    EXPECT_CALL(auth, Hasher_(auth.login, auth.pass, auth.pass_hash)).Times(AtLeast(1));
    EXPECT_EQ(auth.Registration(auth.login, auth.pass), true);
}

TEST(Hasher, RemoveUser) {
    MockAuth auth;
    EXPECT_CALL(auth, Hasher_(auth.login, auth.pass, auth.pass_hash)).Times(AtLeast(1));
    EXPECT_EQ(auth.RemoveUser(auth.login, auth.pass), false);
}

#endif