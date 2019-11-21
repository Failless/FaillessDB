#ifndef FAILLESS_FILESYSTEM_INTERFACE_H
#define FAILLESS_FILESYSTEM_INTERFACE_H

namespace failless {
namespace client {
namespace filesystem {

class FileSystemInterface {
public:
    FileSystemInterface() = default;
    ~FileSystemInterface() = default;

    virtual size_t ReadFile(uintptr_t data) = 0;

    virtual size_t WriteFile(uintptr_t data) = 0;
};

} // namespace filesystem
} // namespace llsscli
} // namespace failless

#endif //FAILLESS_FILESYSTEM_INTERFACE_H
