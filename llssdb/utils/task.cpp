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

}  // namespace utils
}  // namespace db
}  // namespace failless

