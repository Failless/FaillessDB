#ifndef FAILLESS_LLSSDB_COMMON_TASK_H_
#define FAILLESS_LLSSDB_COMMON_TASK_H_

#include <boost/chrono/system_clocks.hpp>
#include <utility>
#include <boost/uuid/uuid.hpp>
#include <memory>
#include <string>
#include "llssdb/common/operators.h"

namespace failless {
namespace db {
namespace common {

class Data {
public:
    explicit Data(short _folder_id = 0,
                  size_t _size = 0,
                  std::shared_ptr<int8_t> _value = nullptr,
                  std::shared_ptr<std::string> _key = nullptr) :
        size(_size),
        value(_value),
        folder_id(_folder_id),
        key(_key) {};

    Data(const Data& data) = default;
    Data& operator=(const Data&) = default;
    ~Data() = default;
    bool operator!=(const Data& r) const;

    size_t size;
    std::shared_ptr<int8_t> value;
    short folder_id;
    std::shared_ptr<std::string> key;
};

class Task {
public:
    Task()
      : query(nullptr),
        command(operators::GET) {}
    Task(short _id, operators _command, std::shared_ptr<std::string> _query)
      : query(_query),
        payload(_id),
        command(_command) {}
    Task(short _id, size_t _size, std::shared_ptr<std::string> _key, std::shared_ptr<int8_t> _value, std::shared_ptr<std::string> _query, operators _command, boost::uuids::uuid _client_id,
            boost::chrono::microseconds _time)
      : query(_query),
        payload(_id, _size, _value, _key),
        command(_command),
        client_id(_client_id),
        time(_time) {}
    explicit Task(std::shared_ptr<std::string> _query, operators _command)
      : query(_query),
        command(_command) {}
    Task(std::shared_ptr<int8_t> _value, size_t _size, operators _command)
      : query(nullptr),
        payload(0, _size, _value),
        command(_command) {}
    Task(std::shared_ptr<int8_t> _value, size_t _size, std::shared_ptr<std::string> _key, operators _command)   // this one is used for tests
      : query(nullptr),
        payload(0, _size, _value, _key),
        command(_command) {}

    Task(const Task& task) = default;
    Task& operator=(const Task&) = default;
    ~Task() = default;
    bool operator==(const Task& r) const;

    std::shared_ptr<std::string> query;
    Data payload;
    operators command;
    boost::uuids::uuid client_id{};
    boost::chrono::microseconds time{};
};

}  // namespace common
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_COMMON_TASK_H_
