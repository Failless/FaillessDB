#ifndef LLSSCLIENT_FILESYSTEM_FILESYSTEM_H_
#define LLSSCLIENT_FILESYSTEM_FILESYSTEM_H_

#include <cstddef>
#include <cstdint>
#include <string>

namespace failless::client::filesystem {

using std::string;

class FileSystemInterface {
public:
    FileSystemInterface() = default;
    ~FileSystemInterface() = default;

    virtual size_t ReadFile(uintptr_t data) = 0;

    virtual size_t WriteFile(uintptr_t data) = 0;
};

class FileSystem : public FileSystemInterface {
public:
    explicit FileSystem(string file_name);

    ~FileSystem() = default;

    size_t ReadFile(uintptr_t data) override;

    size_t WriteFile(uintptr_t data) override;

private:
    string file_name_;
};
}  // failless::llsscli::filesystem

#endif // LLSSCLIENT_FILESYSTEM_FILESYSTEM_H_
