//
// Created by sergey on 19.11.2019.
//

#ifndef FAILLESS_TEST_CONFIG_H
#define FAILLESS_TEST_CONFIG_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "llssdb/config/configmanager.h"

using ::testing::_;
using ::testing::AtLeast;


class MockConfig : public ConfigManager {
public:
    std::string config;
    MOCK_METHOD1(ParseConfig_, std::string(std::string config));
    MockConfig() {
        config = "config_info";
    }
};

TEST(ReadConfig_, Parser) {
    MockConfig conf;
    EXPECT_CALL(conf, ParseConfig_(conf.config)).Times(AtLeast(1));
    EXPECT_EQ(conf.ReadConfig_(), EXIT_SUCCESS);
}

#endif //FAILLESS_TEST_CONFIG_H
