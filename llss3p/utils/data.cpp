#include "llss3p/utils/data.h"
#include <cstring>

namespace failless {
namespace common {
namespace utils {

bool Data::operator!=(const Data& r) const {
    if (size != r.size) {
        return true;
    }
    for (size_t i = 0; i < size; ++i) {
        if (value[i] != r.value[i]) {
            return true;
        }
    }
    if (folder_id != r.folder_id) {
        return true;
    }
    return key != r.key;
}

Data& Data::operator=(const Data& data) {
    if (this != &data) {
        size = data.size;
        folder_id = data.folder_id;
        key = data.key;
//        value = new uint8_t[size];
        // TODO: remove
        value = std::move(data.value);
//        memcpy(value, data.value, sizeof(uint8_t) * size);
    }
    return *this;
}

}  // namespace utils
}  // namespace common
}  // namespace failless
