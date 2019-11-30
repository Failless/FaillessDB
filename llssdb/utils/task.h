#ifndef FAILLESS_LLSSDB_COMMON_TASK_H_
#define FAILLESS_LLSSDB_COMMON_TASK_H_

#include <boost/chrono/system_clocks.hpp>
#include <boost/uuid/uuid.hpp>
#include <string>
#include "llss3p/enums/operators.h"

namespace failless {
namespace db {
namespace utils {

class Data {
 public:
    //    Data() : value(nullptr), size(0), folder_id(0) {};
    explicit Data(short _folder_id = 0, size_t _size = 0, int8_t* _value = nullptr)
        : size(_size), value(_value), folder_id(_folder_id), key(nullptr){};
    Data(short _folder_id, size_t _size, int8_t* _value, std::string* _key)
        : size(_size), value(_value), folder_id(_folder_id), key(_key){};

    Data(const Data& data) = default;
    Data& operator=(const Data&) = default;
    ~Data() = default;
    bool operator!=(const Data& r) const;

    void Destruct() {
        delete (value);
        delete (key);
    }

    size_t size;
    int8_t* value;
    short folder_id;
    std::string* key;
};

// I rewrote it on class because I had an error "static assertion
// failed:(boost::has_trivial_destructor<T>::value)" I don't think that it is a good idea but as I
// saw in the documentation boost have only one way to use lockfree queue

// TODO(rowbotman): Task doesn't provide a std::string key, so plz do it
// or do i have to parse it from query since there could be JOINs?
class Task {
 public:
    Task() : query(nullptr), command(common::enums::operators::GET) {}
    Task(short _id, common::enums::operators _command, std::string* _query)
        : query(_query), payload(_id), command(_command) {}
    Task(short _id, size_t _size, std::string* _key, int8_t* _data, std::string* _query,
         common::enums::operators _command, boost::uuids::uuid _client_id,
         boost::chrono::microseconds _time)
        : query(_query),
          payload(_id, _size, _data, _key),
          command(_command),
          client_id(_client_id),
          time(_time) {}
    explicit Task(std::string* _query, common::enums::operators _command)
        : query(_query), command(_command) {}
    Task(int8_t* _data, size_t _size, common::enums::operators _command)
        : query(nullptr), payload(0, _size, _data), command(_command) {}
    Task(int8_t* _data, size_t _size, std::string* _key,
         common::enums::operators _command)  // this one is used for tests
        : query(nullptr), payload(0, _size, _data, _key), command(_command) {}

    Task(const Task& task) = default;
    Task& operator=(const Task&) = default;
    ~Task() = default;
    bool operator==(const Task& r) const;

    void Destruct() {
        delete (query);
        payload.Destruct();
    }

    std::string* query;
    Data payload;
    common::enums::operators command;
    boost::uuids::uuid client_id{};
    boost::chrono::microseconds time{};
};

}  // namespace utils
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_COMMON_TASK_H_