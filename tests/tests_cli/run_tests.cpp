#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "tests/tests_cli/serialization_tests.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
