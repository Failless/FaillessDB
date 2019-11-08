#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/tests/test_server.h"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
