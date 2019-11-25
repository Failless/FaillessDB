#include "llsscli/filesystem/filesystem.h"

namespace failless {
namespace client {
namespace filesystem {

FileSystem::FileSystem() {

}

size_t FileSystem::ReadFile(std::string file_path) {
    std::ifstream instream(file_path, std::ios::in | std::ios::binary);
    payload_ = new std::vector<uint8_t>((std::istreambuf_iterator<char>(instream)), std::istreambuf_iterator<char>());
    return 0;
}

size_t FileSystem::WriteFile(uintptr_t data) {
    return 0;
}

std::vector<uint8_t>* FileSystem::GetPayload() {
    return payload_;
}

FileSystem::~FileSystem() {
    delete payload_;
}

size_t FileSystem::CleanPayload() {
    delete payload_;
    payload_ = nullptr;
    return 0;
}

}  // namespace filesystem
}  // namespace client
}  // namespace failless