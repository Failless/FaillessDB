#ifndef FAILLESS_LLSSDB_ENGINE_TASK_H_
#define FAILLESS_LLSSDB_ENGINE_TASK_H_

#include <boost/chrono/system_clocks.hpp>
#include <boost/uuid/random_generator.hpp>
#include <utility>
#include <boost/uuid/uuid.hpp>
#include <string>

namespace failless {
namespace db {
namespace common {

// I rewrote it on class because I had an error "static assertion
// failed:(boost::has_trivial_destructor<T>::value)" I don't think that it is a good idea but as I
// saw in the documentation boost have only one way to use lockfree queue
class Task {
 public:
    size_t data_size = 0;
    int8_t *data = nullptr;
    std::string query;
    boost::uuids::uuid client_id{};
    boost::chrono::microseconds time{};
    bool operator==(const Task &r) const;
    Task() : data_size(0), data(nullptr), query(""), client_id({}) {}
    Task(size_t _size, int8_t* _data, std::string _query, boost::uuids::uuid _client_id, boost::chrono::microseconds _time) :
        data_size(_size), data(_data), query(std::move(_query)), client_id(_client_id), time(_time) {}
    ~Task() = default;
    Task(const Task& task) {};
    Task(const Task&&) {};
    Task & operator=(const Task& task) {}
    Task & operator=(const Task&& task) {}
};

}  // namespace common
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_ENGINE_TASK_H_
