#ifndef TESTS_DB_CONFIG_H
#define TESTS_DB_CONFIG_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/utils/settings.h"
#include "llssdb/utils/config_manager.h"

using ::testing::_;
using ::testing::AtLeast;

/**
 * This test simulating situation,
 * when path to config is invalid
 * (checking default config).
 */
TEST(ConfigManager, TestDefaultConfig) {
    failless::db::utils::Settings settings;
    failless::db::utils::ConfigManager conf("not a valid path");
    conf.Initialize(settings);

    EXPECT_EQ(settings.bind, "localhost");
    EXPECT_EQ(settings.setonly, false);
    EXPECT_EQ(settings.readonly, false);
    EXPECT_EQ(settings.lua, false);
    EXPECT_EQ(settings.using_email, false);
    EXPECT_EQ(settings.port, 8888);
}

class MockConfig : public failless::db::utils::ConfigManager {
 public:
    explicit MockConfig(const char *path);

    const char *fake_path;
    MOCK_METHOD2(WriteToSettings_, void(std::string settings, std::string stream));
};

MockConfig::MockConfig(const char *path) : ConfigManager(path) { this->fake_path = path; }

TEST(WriteToSettings_, Parser) {
    MockConfig conf("path");
    failless::db::utils::Settings settings;
    std::string settings_fake;
    std::string fake_stream;

    EXPECT_CALL(conf, WriteToSettings_(settings_fake, fake_stream)).Times(AtLeast(1));

    EXPECT_EQ(conf.Initialize(settings), true);
}

#endif  // TESTS_DB_CONFIG_H
