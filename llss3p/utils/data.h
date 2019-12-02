#ifndef FAILLESS_LLSS3P_UTILS_DATA_H_
#define FAILLESS_LLSS3P_UTILS_DATA_H_

#include <string>

namespace failless {
namespace common {
namespace utils {

class Data {
 public:
//    Data() : size(0), value(nullptr), folder_id(0), key(nullptr){};
    Data(short _folder_id = 0, size_t _size = 0, uint8_t* _value = nullptr)
        : size(_size), value(_value), folder_id(_folder_id), key(nullptr){};
    Data(short _folder_id, size_t _size, uint8_t* _value, std::string* _key)
        : size(_size), value(_value), folder_id(_folder_id), key(_key){};

    Data(const Data& data) = default;
    Data& operator=(const Data&) = default;
    ~Data() = default;
    bool operator!=(const Data& r) const;

    void Destruct() {
        delete (value);
        delete (key);
    }

    size_t size;
    uint8_t* value;
    short folder_id;
    std::string* key;
};

}  // namespace utils
}  // namespace common
}  // namespace failless

#endif  // FAILLESS_LLSS3P_UTILS_DATA_H_
