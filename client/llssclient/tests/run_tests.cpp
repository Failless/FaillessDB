#include <string>
#include "llssclient/core/client.h"
#include "llssclient/network/mp_client.h"
#include "llssclient/filesystem/filesystem.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace failless::client::tests {

    using ::testing::AtLeast;
    using ::testing::DoAll;
    using ::testing::Return;
    using ::testing::SetArgReferee;

    using std::string;

    using namespace failless::client::client_core;

    class mock_client : public client_interface {
    public:

        MOCK_METHOD1(read_query, size_t(string
                query ));

        MOCK_METHOD0(send, size_t());

        MOCK_METHOD2(params, size_t(int
                argc, char * *arg ));
    private:
        MOCK_METHOD1(parse_query, size_t(string
                query ));
    };

    class best_client {
        mock_client *client;
    public:
        best_client(mock_client *client) : client(client) {}

        bool get_data(string) {
            string test_query = "test";
            client->read_query(test_query);
            return true;
        }
    };

    TEST(best_client_test, can_read_query) {
        mock_client mock_client;
        string test_query = "test";
        EXPECT_CALL(mock_client, read_query(test_query)).Times(AtLeast(1));

        best_client best_client(&mock_client);

    }

    int main(int argc, char **argv) {
        ::testing::InitGoogleMock(&argc, argv);
        return RUN_ALL_TESTS();
    }

}
