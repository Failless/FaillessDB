#include <iostream>
#include "llssdb/engine/server_manager.h"
#include "llssdb/folder/file_system.h"

int main(int argc, char **argv) {
    std::unique_ptr<failless::db::folder::FileSystemInterface> file_system(
        new failless::db::folder::FileSystem("../cmake-build-debug"));
    std::string kKey = "key";
    int8_t data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    file_system->Set(kKey, data);
    return 0;
}
