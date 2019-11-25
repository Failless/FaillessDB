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

    size_t ReadFile(string file_path) override;
    size_t WriteFile(uintptr_t data) override;

    vector<uint8_t>* GetPayload();
    size_t CleanPayload() override;

private:
    string file_name_ = "";
    vector<uint8_t>* payload_ = nullptr;
};

} // namespace filesystem
} // namespace llsscli
} // namespace failless

#endif  // LLSSCLI_FILESYSTEM_FILESYSTEM_H_
