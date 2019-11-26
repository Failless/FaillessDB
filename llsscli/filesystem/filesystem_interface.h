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

using std::string;
using std::vector;
using std::ios;
using std::ifstream;
using std::unique_ptr;
using std::streampos;

using namespace failless::client::config;

class FileSystemInterface : boost::noncopyable {
public:
    FileSystemInterface() = default;
    virtual ~FileSystemInterface() = default;

    virtual size_t ReadFile(string file_path, unique_ptr< uint8_t[] >& payload) = 0;
    virtual size_t WriteFile(string file_path, size_t file_size, unique_ptr< uint8_t[] >& payload) = 0;

};

} // namespace filesystem
} // namespace llsscli
} // namespace failless

#endif //FAILLESS_FILESYSTEM_INTERFACE_H
