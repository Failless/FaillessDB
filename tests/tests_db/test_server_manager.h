#ifndef TESTS_DB_TEST_SERVER_WORKER_H_
#define TESTS_DB_TEST_SERVER_WORKER_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "llssdb/engine/server_manager.h"

namespace failless::db::tests {

using ::testing::_;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;


}  // namespace failless::db::tests

#endif  // TESTS_DB_TEST_SERVER_WORKER_H_
