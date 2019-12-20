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
    //    int16_t calls;    // counts how many times this value was called
    explicit InMemoryData(std::vector<uint8_t> _value, size_t _size = 0, bool flag = false)
        : value(std::move(_value)), size(_size), in_memory(flag) {};
    // TODO(EgorBedov): i don't think std::move is a right solution

    InMemoryData(InMemoryData&& other) noexcept
      : value(std::move(other.value)),
        size(other.size),
        in_memory(true) {};
};

}  // namespace folder
}  // namespace db
}  // namespace failless

#endif // FAILLESS_LLSSDB_FOLDER_VALUE_INFO_H
