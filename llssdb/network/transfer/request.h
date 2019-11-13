#ifndef LLSSDB_NETWORK_TRANSFER_REQUEST_H_
#define LLSSDB_NETWORK_TRANSFER_REQUEST_H_

#include "llssdb/engine/task.h"
#include <boost/core/noncopyable.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <cstdint>
#include <string>

namespace failless {
namespace db {
namespace network {
namespace transfer {

enum request_type {
  SELECT = 0,
  CREATE,
  UPDATE,
  DELETE,
};

class Request : boost::noncopyable {
public:
  Request() = default;
  Request(const boost::uuids::uuid &client_id, std::string token,
          request_type type);
  ~Request() = default;

  void SetData(int8_t *data, size_t size) {
    data_ = data;
    size_ = size;
  }
  engine::Task GetData() {
    boost::uuids::random_generator generator;
    boost::uuids::uuid client_id = generator();
    auto now = boost::chrono::system_clock::now();
    auto ms = boost::chrono::time_point_cast<boost::chrono::milliseconds>(now);
    return engine::Task {
      0, nullptr, "query", client_id,
          boost::chrono::microseconds(ms.time_since_epoch().count())
    };
  }

protected:
  int8_t *data_ = nullptr;
  size_t size_ = 0;
  request_type type_{};
};

} // namespace transfer
} // namespace network
} // namespace llssdb
} // namespace failless

#endif // LLSSDB_NETWORK_TRANSFER_REQUEST_H_