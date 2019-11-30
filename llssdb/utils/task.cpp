#include "llssdb/utils/task.h"

namespace failless {
namespace db {
namespace utils {

bool Task::operator==(const Task &r) const {
    if (payload != r.payload) {
        return false;
    }
    if (query != r.query) {
        return false;
    }
    if (client_id != r.client_id) {
        return false;
    }
    return !(time != r.time);
}

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
}  // namespace db
}  // namespace failless
