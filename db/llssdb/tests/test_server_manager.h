#ifndef LLSSDB_TESTS_TEST_SERVER_WORKER_H
#define LLSSDB_TESTS_TEST_SERVER_WORKER_H


#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/engine/server_manager.h"

namespace failless::db::tests {

using ::testing::_;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;


} // namespace failless::db::tests


#endif // LLSSDB_TESTS_TEST_SERVER_WORKER_H
