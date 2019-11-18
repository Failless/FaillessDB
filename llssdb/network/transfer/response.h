#ifndef LLSSDB_NETWORK_TRANSFER_RESPONSE_H_
#define LLSSDB_NETWORK_TRANSFER_RESPONSE_H_

#include "llssdb/network/transfer/request.h"

namespace failless {
namespace db {
namespace network {

class Response {
 public:
    Response() = default;
    explicit Response(const Request &request);
    explicit Response(const common::Task &task) {
        data_ = task.data;
        size_ = task.data_size;
    }

 protected:
    int8_t *data_ = nullptr;
    size_t size_{};
    boost::uuids::uuid uuid{};
};

}  // namespace network
}  // namespace db
}  // namespace failless

#endif  // LLSSDB_NETWORK_TRANSFER_RESPONSE_H_
