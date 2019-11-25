#ifndef FAILLESS_FILESYSTEM_INTERFACE_H
#define FAILLESS_FILESYSTEM_INTERFACE_H

#include <string>
#include <fstream>
#include <vector>

#include "llsscli/config/config.h"

namespace failless {
namespace client {
namespace filesystem {

using std::string;
using std::vector;

using namespace failless::client::config;

class FileSystemInterface {
public:
    FileSystemInterface() = default;
    ~FileSystemInterface() = default;

    virtual size_t ReadFile(string file_path) = 0;
    virtual size_t WriteFile(uintptr_t data) = 0;
    virtual size_t CleanPayload() = 0;
};

} // namespace filesystem
} // namespace llsscli
} // namespace failless

#endif //FAILLESS_FILESYSTEM_INTERFACE_H
