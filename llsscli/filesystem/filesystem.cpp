#include "llsscli/filesystem/filesystem.h"

namespace failless {
namespace client {
namespace filesystem {

FileSystem::FileSystem() {

}

// to vector https://gist.github.com/looopTools/64edd6f0be3067971e0595e1e4328cbc
// to vector https://stackoverflow.com/questions/15138353/how-to-read-a-binary-file-into-a-vector-of-unsigned-chars
// to vector with write http://www.cplusplus.com/forum/beginner/197490/
// about unique_ptr https://www.daniweb.com/programming/software-development/threads/456747/reading-binary-files-in-c

size_t FileSystem::ReadFile(std::string file_path, std::unique_ptr< uint8_t[] >& payload) {
    std::ifstream r_file(file_path, std::ios::binary);
    r_file.unsetf(std::ios::skipws);
    r_file.seekg( 0, std::ios::end );
    std::streampos len = r_file.tellg();
    payload.reset( new uint8_t [len] ) ;
    r_file.seekg(0, std::ios::beg);
    r_file.read(reinterpret_cast<char *>(payload.get()), len);
    r_file.close();
    return 0;
}

size_t FileSystem::WriteFile(std::string file_path, size_t file_size, std::unique_ptr< uint8_t[] >& payload) {
    payload.reset( new uint8_t [file_size] ) ;
    std::ofstream w_file(file_path, std::ios::binary);
    w_file.write(reinterpret_cast<char *>(payload.get()), file_size);
    w_file.close();
    return 0;
}

FileSystem::~FileSystem() {}

}  // namespace filesystem
}  // namespace client
}  // namespace failless