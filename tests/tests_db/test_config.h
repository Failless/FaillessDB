//
// Created by sergey on 19.11.2019.
//

#ifndef FAILLESS_TEST_CONFIG_H
#define FAILLESS_TEST_CONFIG_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/utils/config_manager.h"
#include "llssdb/common/settings.h"

using ::testing::_;
using ::testing::AtLeast;

class MockConfig : public failless::db::utils::ConfigManager {
public:
    explicit MockConfig(const char *path);

    const char *fake_path;
    MOCK_METHOD2(WriteToSettings_, void(std::string
            settings, std::string
            stream));
};

MockConfig::MockConfig(const char *path) : ConfigManager(path) {
    this->fake_path = path;
}

TEST(WriteToSettings_, Parser) {
    MockConfig conf("path");
    failless::db::common::Settings settings;
    std::string settings_fake;
    std::string fake_stream;

    EXPECT_CALL(conf, WriteToSettings_(settings_fake, fake_stream)).Times(AtLeast(1));

    EXPECT_EQ(conf.Initialize(settings), true);

}

#endif //FAILLESS_TEST_CONFIG_H
