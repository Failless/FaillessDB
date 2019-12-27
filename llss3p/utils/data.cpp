#include "llss3p/utils/data.h"
#include <cstring>

namespace failless {
namespace common {
namespace utils {

Data& Data::operator=(const Data& data) {
    if (this != &data) {
        size = data.size;
        folder_id = data.folder_id;
        key = data.key;
        value = std::move(data.value);
    }
    return *this;
}

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

bool Data::operator==(const Data& r) const {
    return !(*this != r);
}

}  // namespace utils
}  // namespace common
}  // namespace failless
