#ifndef FAILLESS_FILESYSTEM_INTERFACE_H
#define FAILLESS_FILESYSTEM_INTERFACE_H

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <boost/noncopyable.hpp>

#include "llsscli/config/config.h"

namespace failless {
namespace client {
namespace filesystem {

class FileSystemInterface : boost::noncopyable {
public:
    virtual ~FileSystemInterface() = default;

    virtual size_t ReadFile(std::string file_path, std::unique_ptr< uint8_t[] >& payload) = 0;
    virtual size_t WriteFile(std::string file_path, size_t file_size, std::unique_ptr< uint8_t[] >& payload) = 0;

};

} // namespace filesystem
} // namespace llsscli
} // namespace failless

#endif //FAILLESS_FILESYSTEM_INTERFACE_H
