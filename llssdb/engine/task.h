#ifndef LLSSDB_ENGINE_TASK_H_
#define LLSSDB_ENGINE_TASK_H_

#include <boost/chrono/system_clocks.hpp>
#include <boost/uuid/uuid.hpp>
#include <string>

namespace failless {
namespace db {
namespace engine {

struct Task {
    size_t data_size = 0;
    int8_t *data = nullptr;
    std::string query;
    boost::uuids::uuid &client_id;
    boost::chrono::microseconds time{};
    bool operator==(const Task &r) const;
};

}  // namespace engine
}  // namespace db
}  // namespace failless

#endif  // LLSSDB_ENGINE_TASK_H_
