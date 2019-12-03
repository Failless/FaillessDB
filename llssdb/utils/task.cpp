#include "llssdb/utils/task.h"

namespace failless::db::utils {

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
        if (value.get()[i] != r.value.get()[i]) {
            return true;
        }
    }
    if (folder_id != r.folder_id) {
        return true;
    }
    return key != r.key;
}

}  // namespace failless
