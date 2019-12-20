#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "tests/tests_db/test_auth.h"
#include "tests/tests_db/test_config.h"
#include "tests/tests_db/test_file_system.h"
#include "tests/tests_db/test_lockfree_queue.h"
#include "tests/tests_db/test_server.h"
#include "tests/tests_db/test_server_manager.h"
#include "tests/tests_db/test_task_worker.h"
#include "tests/tests_db/mocks.h"

#include "llss3p/utils/boost_logger.h"

int main(int argc, char **argv) {
    
    failless::db::tests::set_test_db_path();
    failless::common::utils::BoostLogger::filter_logging(boost::log::trivial::debug);

    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
