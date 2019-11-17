#ifndef LLSSCLI_FILESYSTEM_FILESYSTEM_H_
#define LLSSCLI_FILESYSTEM_FILESYSTEM_H_

#include <string>

namespace failless::client::filesystem {

class FileSystemInterface {
 public:
    FileSystemInterface() = default;
    ~FileSystemInterface() = default;

    virtual size_t ReadFile(uintptr_t data) = 0;

    virtual size_t WriteFile(uintptr_t data) = 0;
};

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

}  // namespace failless::client::filesystem

#endif  // LLSSCLI_FILESYSTEM_FILESYSTEM_H_
