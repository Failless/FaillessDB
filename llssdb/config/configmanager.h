//
// Created by sergey on 19.11.2019.
//

#ifndef FAILLESS_CONFIGMANAGER_H
#define FAILLESS_CONFIGMANAGER_H

#include "llssdb/config/settings.h"

class ConfigManager {
public:
    ConfigManager() = default;
    ~ConfigManager() = default;
    struct Settings Config;
    bool ReadConfig_();

private:
    virtual std::string ParseConfig_(std::string config);  // string?
    // bool GetUserInfo_(); necessary?
};

#endif //FAILLESS_CONFIGMANAGER_H
