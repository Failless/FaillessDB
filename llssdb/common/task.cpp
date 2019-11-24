#include "llssdb/common/task.h"

namespace failless {
namespace db {
namespace common {

bool Task::operator==(const Task &r) const {
    if ( !(payload == r.payload) ) {
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

bool Data::operator==(const failless::db::common::Data &r) const {
    if ( size != r.size ) {
        return false;
    }
    for ( size_t i = 0; i < size; ++i ) {
        if ( value[i] != r.value[i] ) {
            return false;
        }
    }
    if ( folder_id != r.folder_id ) {
        return false;
    }
    if ( key != r.key ) {
        return false;
    }
}

}  // namespace common
}  // namespace db
}  // namespace failless
