#ifndef FAILLESS_FILESYSTEM_INTERFACE_H
#define FAILLESS_FILESYSTEM_INTERFACE_H

#include <string>
#include <boost/noncopyable.hpp>

namespace failless {
namespace client {
namespace filesystem {

class FileSystemInterface : boost::noncopyable {
public:
    virtual ~FileSystemInterface() = default;

    virtual size_t ReadFile(std::string file_path, std::unique_ptr< std::vector<unsigned char> >& payload, std::streampos& len) = 0;
    virtual size_t WriteFile(std::string file_path, std::unique_ptr< std::vector<unsigned char> >& payload) = 0;

};

} // namespace filesystem
} // namespace llsscli
} // namespace failless

#endif //FAILLESS_FILESYSTEM_INTERFACE_H
