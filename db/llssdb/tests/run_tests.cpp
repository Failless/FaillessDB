#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "llssdb/tests/test_auth.h"
#include "llssdb/tests/test_server.h"
#include "llssdb/tests/test_file_system.h"
#include "llssdb/tests/test_task_worker.h"
#include "llssdb/tests/test_server_manager.h"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
