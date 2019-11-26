#ifndef LLSSCLI_FILESYSTEM_FILESYSTEM_H_
#define LLSSCLI_FILESYSTEM_FILESYSTEM_H_

#include "llsscli/filesystem/filesystem_interface.h"

namespace failless {
namespace client {
namespace filesystem {

class FileSystem : public FileSystemInterface {
public:
    FileSystem() = default;
    ~FileSystem() override;

    size_t ReadFile(std::string file_path, std::unique_ptr< uint8_t[] >& payload) override;
    size_t WriteFile(std::string file_path, size_t file_size, std::unique_ptr< uint8_t[] >& payload) override;

private:
    std::string file_name_ = "";
};

} // namespace filesystem
} // namespace llsscli
} // namespace failless

#endif  // LLSSCLI_FILESYSTEM_FILESYSTEM_H_
