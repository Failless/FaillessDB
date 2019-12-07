#ifndef FAILLESS_LLSS3P_UTILS_DATA_H_
#define FAILLESS_LLSS3P_UTILS_DATA_H_

#include <msgpack.hpp>
#include <string>

namespace failless {
namespace common {
namespace utils {

struct Data {
    size_t size = 0;
    std::vector<uint8_t> value{};
    short folder_id = 0;
    std::string key{};
    Data(short _folder_id, size_t _size, std::vector<uint8_t>& _value)
        : size(_size), value(_value), folder_id(_folder_id), key(""){};
    Data(short _folder_id, size_t _size)
        : size(_size), folder_id(_folder_id), key(""){};
    Data(short _folder_id, size_t _size, std::vector<uint8_t>& _value, std::string* _key)
        : size(_size), value(_value), folder_id(_folder_id), key(*_key){};

    Data() = default;
    Data(const Data& data) = default;
    Data& operator=(const Data&);
    ~Data() = default;
    bool operator!=(const Data& r) const;
    bool operator==(const Data& r) const;

    void Destruct() {
    }

    MSGPACK_DEFINE_MAP(size, value, folder_id, key);

};

}  // namespace utils
}  // namespace common
}  // namespace failless

#endif  // FAILLESS_LLSS3P_UTILS_DATA_H_
