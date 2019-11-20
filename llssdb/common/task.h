#ifndef FAILLESS_LLSSDB_ENGINE_TASK_H_
#define FAILLESS_LLSSDB_ENGINE_TASK_H_

#include <boost/chrono/system_clocks.hpp>
#include <boost/uuid/uuid.hpp>
#include <string>
#include "llssdb/common/operators.h"

namespace failless {
namespace db {
namespace common {

// I rewrote it on class because I had an error "static assertion
// failed:(boost::has_trivial_destructor<T>::value)" I don't think that it is a good idea but as I
// saw in the documentation boost have only one way to use lockfree queue
class Task {
 public:
    Task() : folder_id(0), data_size(0), data(nullptr), query(nullptr), command(operators::GET) {}
    Task(int _id, operators _command, std::string* _query)
        : folder_id(_id), data_size(0), data(nullptr), query(_query), command(_command) {}
    Task(int _id, size_t _size, int8_t* _data, std::string* _query, boost::uuids::uuid _client_id,
         boost::chrono::microseconds _time)
        : folder_id(_id),
          data_size(_size),
          data(_data),
          query(_query),
          command(operators::GET),
          client_id(_client_id),
          time(_time) {}

    Task(operators _command, std::string* _query)
        : folder_id(0), data_size(0), data(nullptr), query(_query), command(_command) {}
    Task(const Task& task) = default;
    Task& operator=(const Task&) = default;
    ~Task() = default;
    bool operator==(const Task& r) const;

    int folder_id;
    size_t data_size = 0;
    int8_t* data = nullptr;
    std::string* query;
    operators command;
    boost::uuids::uuid client_id{};
    boost::chrono::microseconds time{};
};

}  // namespace common
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_ENGINE_TASK_H_
