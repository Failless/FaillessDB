#include "llssdb/common/task.h"

namespace failless {
namespace db {
namespace common {

bool Task::operator==(const Task &r) const {
    if ( payload != r.payload ) {
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

Task::~Task() {
    delete(query);
}

bool Data::operator!=(const failless::db::common::Data &r) const {
    if ( size != r.size ) {
        return true;
    }
    for ( size_t i = 0; i < size; ++i ) {
        if ( value[i] != r.value[i] ) {
            return true;
        }
    }
    if ( folder_id != r.folder_id ) {
        return true;
    }
    return key != r.key;
}

Data::~Data() {
    delete(value);
    delete(key);
}

}  // namespace common
}  // namespace db
}  // namespace failless
