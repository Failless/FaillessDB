#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "tests/tests_cli/serialization/test_client_serializer_integration.h"
#include "tests/tests_cli/serialization/test_serializer_mock.h"
#include "tests/tests_cli/serialization/test_serializer_unit.h"

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
