#ifndef FAILLESS_VALUE_INFO_H
#define FAILLESS_VALUE_INFO_H

//#include <utility>

namespace failless::db::folder {

struct ValueInfo {
    int8_t* value;
    size_t size;
    bool in_memory;
//    int16_t calls;    // counts how many times this value was called

    explicit ValueInfo(int8_t* _value = nullptr, bool flag = false) :
        value(_value),
        in_memory(flag) {};
};

}

#endif //FAILLESS_VALUE_INFO_H
