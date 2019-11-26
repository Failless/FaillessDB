#ifndef LLSSCLI_FILESYSTEM_FILESYSTEM_H_
#define LLSSCLI_FILESYSTEM_FILESYSTEM_H_

#include "llsscli/filesystem/filesystem_interface.h"

namespace failless {
namespace client {
namespace filesystem {

class FileSystem : public FileSystemInterface {
public:
    FileSystem();
    ~FileSystem();

    size_t ReadFile(string file_path, unique_ptr< uint8_t[] >& payload) override;
    size_t WriteFile(string file_path, size_t file_size, unique_ptr< uint8_t[] >& payload) override;

private:
    string file_name_ = "";
};

} // namespace filesystem
} // namespace llsscli
} // namespace failless

#endif  // LLSSCLI_FILESYSTEM_FILESYSTEM_H_
