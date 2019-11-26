#include "llssdb/utils/config_manager.h"


bool failless::db::utils::ConfigManager::Initialize(failless::db::common::Settings &settings) {
    // no need to call close for std::ifstream
    const char *config_path = "../../failless.conf";
    if (access(config_path, R_OK) == -1) {
        std::cerr << "No such file";
        return false;
    }
    std::ifstream cFile(config_path);
    if (cFile.is_open()) {
        WriteToSettings_(settings, cFile);
    } else {
        std::cerr << "Couldn't find file or open for reading\n";
        return false;
    }
    return true;
}

void  // auto-formatting is so strange sometimes..
failless::db::utils::ConfigManager::WriteToSettings_(failless::db::common::Settings &settings, std::ifstream &cFile) {
    std::string line;
    while (getline(cFile, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
        if (line[0] == '#' || line.empty()) {
            continue;
        }
        auto delimiterPos = line.find("=");
        std::string name = line.substr(0, delimiterPos);
        auto value = line.substr(delimiterPos + 1);
        if (name == "bind") {
            settings.bind = value;
        } else if (name == "data_path") {
            settings.data_path = value;
        } else if (name == "threads") {
            settings.threads = std::stoi(value);
        } else if (name == "tcp-keepalive") {
            settings.tcp_keepalive = std::stoi(value);
        } else if (name == "max_bytes") {
            settings.max_bytes = std::stoi(value);
        } else if (name == "query_max") {
            settings.query_max = std::stoi(value);
        } else if (name == "port") {
            settings.port = std::stoi(value);
        } else if (name == "setonly") {
            if (std::stoi(value) == 1) {
                settings.setonly = true;
            }
        } else if (name == "readonly") {
            if (std::stoi(value) == 1) {
                settings.readonly = true;
            }
        } else if (name == "lua") {
            if (std::stoi(value) == 1) {
                settings.lua = true;
            }
        } else if (name == "users_to_table") {
            settings.users_to_table = std::stoi(value);
        } else if (name == "email") {
            settings.using_email = true;
            settings.email = value;
        }
    }
}