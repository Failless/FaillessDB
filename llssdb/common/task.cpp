#include "llssdb/common/task.h"

namespace failless {
namespace db {
namespace common {

bool Task::operator==(const Task &r) const {
    if (data_size != r.data_size) {
        return false;
    }
    for (size_t i = 0; i < data_size; ++i) {
        if (data[i] != r.data[i]) {
            return false;
        }
    }
    if (query != r.query) {
        return false;
    }
    if (client_id != r.client_id) {
        return false;
    }
    return !(time != r.time);
}

}  // namespace common
}  // namespace db
}  // namespace failless
