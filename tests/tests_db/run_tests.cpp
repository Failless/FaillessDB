#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "tests/tests_db/test_server.h"
#include "tests/tests_db/test_task_worker.h"
#include "tests/tests_db/test_server_manager.h"
#include "tests/tests_db/test_auth.h"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}