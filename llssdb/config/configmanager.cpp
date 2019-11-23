//
// Created by sergey on 19.11.2019.
//

#include "llssdb/config/configmanager.h"

bool ConfigManager::ReadConfig_() {
    // reading
    std::string config = "info from file";

    ParseConfig_(config);
    return true;
}

std::string ConfigManager::ParseConfig_(std::string config) {
    return config;
}
