#ifndef LLSSDB_NETWORK_TRANSFER_RESPONSE_H_
#define LLSSDB_NETWORK_TRANSFER_RESPONSE_H_

#include "llssdb/network/transfer/request.h"
#include <boost/core/noncopyable.hpp>

namespace failless {
namespace db {
namespace network {
namespace transfer {

class Response : boost::noncopyable {
public:
  Response() = default;
  explicit Response(const Request &request);
  explicit Response(const engine::Task& task) {
    data_ = task.data;
    size_ = task.data_size;
  }

protected:
  int8_t *data_;
  size_t size_;
  boost::uuids::uuid uuid{};
};

} // namespace transfer
} // namespace network
} // namespace db
} // namespace failless

#endif // LLSSDB_NETWORK_TRANSFER_RESPONSE_H_
