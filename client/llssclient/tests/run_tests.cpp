#include <string>
#include "llssclient/core/client.h"
#include "llssclient/filesystem/filesystem.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace failless::client::tests {

    using ::testing::AtLeast;
    using ::testing::DoAll;
    using ::testing::Return;
    using ::testing::SetArgReferee;

    using std::string;

    using namespace failless::client::core;

    class MockClient : public ClientInterface {
    public:
        MockClient() : ClientInterface() {};
        MOCK_METHOD1(ReadQuery, size_t(string  query));
        MOCK_METHOD0(Send, size_t());
        MOCK_METHOD2(Params, size_t(int argc, char **arg));
    private:
        MOCK_METHOD1(ParseQuery_, size_t(string query));
    };

    TEST(client_test, can_read_query) {
        MockClient mock_client;
        string test_query = "test";
        EXPECT_CALL(mock_client, ReadQuery(test_query)).Times(AtLeast(1));
        EXPECT_EQ(mock_client.Test("test"), EXIT_SUCCESS);
    }

    int main(int argc, char **argv) {
        ::testing::InitGoogleMock(&argc, argv);
        return RUN_ALL_TESTS();
    }

}
