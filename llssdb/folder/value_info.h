#ifndef LLSSDB_FOLDER_VALUE_INFO_H
#define LLSSDB_FOLDER_VALUE_INFO_H

#include <memory>

namespace failless::db::folder {

struct ValueInfo {
    explicit ValueInfo(std::shared_ptr<int8_t> _value = nullptr, size_t _size = 0, bool _flag = false) :
        value(_value),
        size(_size),
        in_memory(_flag) {};

    ~ValueInfo() = default;

    std::shared_ptr<int8_t> value;
    size_t size;
    bool in_memory;
//    int16_t calls;    // counts how many times this value was called
};

}

#endif // LLSSDB_FOLDER_VALUE_INFO_H
