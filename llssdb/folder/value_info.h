#ifndef FAILLESS_LLSSDB_FOLDER_VALUE_INFO_H_
#define FAILLESS_LLSSDB_FOLDER_VALUE_INFO_H_

namespace failless {
namespace db {
namespace folder {

struct ValueInfo {
    uint8_t* value;
    size_t size;
    bool in_memory;
    //    int16_t calls;    // counts how many times this value was called

    explicit ValueInfo(uint8_t* _value = nullptr, bool flag = false)
        : value(_value), size(0), in_memory(flag){};
};

}  // namespace folder
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_FOLDER_VALUE_INFO_H_
