#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace failless::client::tests {

using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;


int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace failless::client::tests
