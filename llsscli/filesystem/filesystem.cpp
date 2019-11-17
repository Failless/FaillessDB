#include "llsscli/filesystem/filesystem.h"
#include <utility>

failless::client::filesystem::FileSystem::FileSystem(std::string file_name)
    : file_name_(std::move(file_name)) {}

size_t failless::client::filesystem::FileSystem::ReadFile(uintptr_t data) {
    return 0;
}

size_t failless::client::filesystem::FileSystem::WriteFile(uintptr_t data) {
    return 0;
}
