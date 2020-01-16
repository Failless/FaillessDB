#ifndef FAILLESS_LLSSDB_FOLDER_VALUE_INFO_H
#define FAILLESS_LLSSDB_FOLDER_VALUE_INFO_H


#include <cstdlib>
#include <vector>

namespace failless {
namespace db {
namespace folder {

struct InMemoryData {
    std::vector<uint8_t> value;
    size_t size;
    bool in_memory;
};

}  // namespace folder
}  // namespace db
}  // namespace failless

#endif // FAILLESS_LLSSDB_FOLDER_VALUE_INFO_H
