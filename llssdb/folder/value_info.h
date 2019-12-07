#ifndef LLSSDB_FOLDER_VALUE_INFO_H
#define LLSSDB_FOLDER_VALUE_INFO_H

#include <memory>
#include <utility>

namespace failless {
namespace db {
namespace folder {

struct ValueInfo {
    uint8_t* value;
    size_t size;
    bool in_memory;
    //    int16_t calls;    // counts how many times this value was called
    explicit ValueInfo(uint8_t* _value = nullptr, size_t _size = 0, bool flag = false)
        : value(_value), size(_size), in_memory(flag){};
};

}  // namespace folder
}  // namespace db
}  // namespace folder

#endif // LLSSDB_FOLDER_VALUE_INFO_H
