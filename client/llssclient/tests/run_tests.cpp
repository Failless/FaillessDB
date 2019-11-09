#include <iostream>
#include "../core/client.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

using namespace client_core;

class mock_client : public client {
public:
    explicit mock_client( uintptr_t interpreter, class mp_client* mp_client, class filesystem* filesystem, char* config ) : client( interpreter, mp_client, filesystem, config ){};
    MOCK_METHOD0(read_query, size_t() );
    MOCK_METHOD0( send, size_t() );
    MOCK_METHOD0( params, size_t() );
private:
    MOCK_METHOD0(parse_query, size_t());
};

TEST(DynamicArrayTest, ReallocateCalledOnce) {
    uintptr_t interpreter = 0;
    mp_client* mp_client = 0;
    filesystem* filesystem = 0;
    char* config = 0;
    mock_client client{interpreter, mp_client, filesystem, config};
    EXPECT_CALL( mock_client, read_query()).Times(AtLeast(1) );
}

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
