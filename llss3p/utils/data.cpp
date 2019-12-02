#include "llss3p/utils/data.h"

namespace failless {
namespace common {
namespace utils {

bool Data::operator!=(const Data &r) const {
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

}  // namespace utils
}  // namespace common
}  // namespace failless
