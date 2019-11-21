#ifndef LLSSCLI_FILESYSTEM_FILESYSTEM_H_
#define LLSSCLI_FILESYSTEM_FILESYSTEM_H_

#include <string>

#include "llsscli/filesystem/filesystem_interface.h"

namespace failless {
namespace client {
namespace filesystem {

class FileSystem : public FileSystemInterface {
public:
    FileSystem() = default;

    explicit FileSystem(std::string file_name);

    ~FileSystem() = default;

    size_t ReadFile(uintptr_t data) override;

    size_t WriteFile(uintptr_t data) override;

private:
    std::string file_name_;
};

} // namespace filesystem
} // namespace llsscli
} // namespace failless

#endif  // LLSSCLI_FILESYSTEM_FILESYSTEM_H_
