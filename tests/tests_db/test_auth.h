#ifndef TESTS_DB_AUTH_H_
#define TESTS_DB_AUTH_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "llss3p/utils/hasher.h"
#include "llssdb/auth/authorization.h"

using ::testing::_;
using ::testing::AtLeast;

namespace failless {
namespace db {
namespace tests {

class MockAuth : public auth::Authorization {
 public:
    std::string login;
    std::string pass;
    unsigned char pass_hash[32]{};
    MOCK_METHOD1(CheckCollisions_, bool(std::string login));
    MOCK_METHOD3(Hasher_, void(const std::string &login, std::string pass, unsigned char *md));
    MockAuth() : Authorization() {
        login = "login";
        pass = "pass";
        // pass_hash = "pass_hash";
    }
};

class RegisterImpl : public ::testing::Test {
 public:
    void SetUp() override {
        auth.reset(new auth::Authorization());
        login = "login";
        password = "password";
    }

    std::shared_ptr<auth::Authorization> auth;
    std::string login;
    std::string password;
};

/**
 * check impossibility of registration two users with one login
 */
TEST_F(RegisterImpl, Registration) {
    // TODO: add tests on validation
    std::string login = "mr_tester";
    std::string pass = "qwerty12345";
    EXPECT_TRUE(auth->Registration(login, pass, -1));
    std::string login2 = "mr_tester";
    std::string pass2 = "qwerty12345";
    EXPECT_FALSE(auth->Registration(login2, pass2, -1));
}

/**
 * check correct work of IsAuth function
 */
TEST_F(RegisterImpl, IsAuth) {
    std::string login = "mr_tester";
    std::string pass = "qwerty12345";
    int folder_id = -1;
    auth->Registration(login, pass, -1);
    EXPECT_TRUE(auth->IsAuth(login, pass, folder_id));
    EXPECT_FALSE(auth->IsAuth(login, pass, 1));
    std::string login2 = "mr_tester2";
    std::string pass2 = "qwerty12345";
    EXPECT_FALSE(auth->IsAuth(login, pass, 0));
}

/**
 * check correct work of RemoveUser function
 */
TEST_F(RegisterImpl, RemoveUser) {
    std::string login = "mr_tester";
    std::string pass = "qwerty12345";
    auth->Registration(login, pass, -1);
    EXPECT_TRUE(auth->RemoveUser(login, pass));
    EXPECT_FALSE(auth->RemoveUser(login, pass));
    std::string login2 = "mr_tester2";
    std::string pass2 = "qwerty12345";
    EXPECT_FALSE(auth->RemoveUser(login, pass));
}

/**
 * check correct work of Hasher_ function
 */
TEST(Registration, HashFunc) {
    std::string pass = "qwerty12345";
    unsigned char array[SHA256_DIGEST_LENGTH]{};
    EXPECT_TRUE(failless::common::utils::SimpleSHA256(pass, array));

    std::string pass2 = "qwerty12345";
    unsigned char array2[SHA256_DIGEST_LENGTH]{};
    EXPECT_TRUE(failless::common::utils::SimpleSHA256(pass, array2));

    std::string pass3 = "qwerty1234567";
    unsigned char array3[SHA256_DIGEST_LENGTH]{};
    EXPECT_TRUE(failless::common::utils::SimpleSHA256(pass3, array3));
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        EXPECT_EQ(array[i], array2[i]);
    }
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        EXPECT_TRUE(array[i] != array3[i]);
    }
}

// MockTests

TEST(Registration, Registration) {
    MockAuth auth;
    EXPECT_CALL(auth, CheckCollisions_(auth.login)).Times(AtLeast(1));
    EXPECT_EQ(auth.Registration(auth.login, auth.pass, -1), true);
}

TEST(Hasher, Registration) {
    MockAuth auth;
    EXPECT_CALL(auth, Hasher_(auth.login, auth.pass, auth.pass_hash)).Times(AtLeast(1));
    EXPECT_EQ(auth.Registration(auth.login, auth.pass, -1), true);
}

TEST(Hasher, RemoveUser) {
    MockAuth auth;
    EXPECT_CALL(auth, Hasher_(auth.login, auth.pass, auth.pass_hash)).Times(AtLeast(1));
    EXPECT_EQ(auth.RemoveUser(auth.login, auth.pass), false);
}

}  // namespace tests
}  // namespace db
}  // namespace failless

#endif  // TESTS_DB_AUTH_H_
