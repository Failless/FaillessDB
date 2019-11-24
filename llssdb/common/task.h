#ifndef FAILLESS_LLSSDB_ENGINE_TASK_H_
#define FAILLESS_LLSSDB_ENGINE_TASK_H_

#include <boost/chrono/system_clocks.hpp>
#include <utility>
#include <boost/uuid/uuid.hpp>
#include <string>
#include "llssdb/common/operators.h"

namespace failless {
namespace db {
namespace common {

class Data {
public:
//    Data() : value(nullptr), size(0), folder_id(0) {};
    explicit Data(short _folder_id = 0, size_t _size = 0, int8_t* _value = nullptr) :
        size(_size),
        value(_value),
        folder_id(_folder_id) {};
    Data(short _folder_id, size_t _size, int8_t* _value, std::string _key) :
        size(_size),
        value(_value),
        folder_id(_folder_id),
        key(std::move(_key)) {};

    Data(const Data& data) = default;
    Data& operator=(const Data&) = default;
    ~Data() = default;
    bool operator==(const Data& r) const;

    size_t size;
    int8_t* value;
    short folder_id;
    std::string key;
};

// I rewrote it on class because I had an error "static assertion
// failed:(boost::has_trivial_destructor<T>::value)" I don't think that it is a good idea but as I
// saw in the documentation boost have only one way to use lockfree queue

// TODO(rowbotman): Task doesn't provide a std::string key, so plz do it
// or do i have to parse it from query since there could be JOINs?
class Task {
public:
    Task()
      : query(nullptr) {}
    Task(short _id, std::string* _query)
      : query(_query),
        payload(_id) {}
    Task(short _id, size_t _size, int8_t* _data, std::string* _query, boost::uuids::uuid _client_id,
            boost::chrono::microseconds _time)
      : query(_query),
        payload(_id, _size, _data),
        client_id(_client_id),
        time(_time) {}

    explicit Task(std::string* _query)
      : query(_query) {}
    Task(int8_t *_data, size_t _size)
      : query(nullptr),
        payload(0, _size, _data) {}
    Task(int8_t *_data, size_t _size, std::string _key)   // this one is used for tests
      : query(nullptr),
        payload(0, _size, _data, _key) {}

    Task(const Task& task) = default;
    Task& operator=(const Task&) = default;
    ~Task() = default;
    bool operator==(const Task& r) const;

    std::string* query;
    Data payload;
    boost::uuids::uuid client_id{};
    boost::chrono::microseconds time{};
};

}  // namespace common
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_ENGINE_TASK_H_
