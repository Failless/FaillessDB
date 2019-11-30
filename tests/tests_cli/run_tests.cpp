#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "tests/tests_cli/test_serializer_mock.h"
#include "tests/tests_cli/test_serializer_unit.h"
#include "tests/tests_cli/test_client_serializer_integration.h"

namespace failless {
namespace client {
namespace test {

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

}  // namespace test
}  // namespace client
}  // namespace failless
