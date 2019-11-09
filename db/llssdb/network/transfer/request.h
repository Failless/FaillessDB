#ifndef LLSSDB_NETWORK_TRANSFER_REQUEST_H_
#define LLSSDB_NETWORK_TRANSFER_REQUEST_H_

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

class Request {
public:
  Request() = default;
  Request(const boost::uuids::uuid &client_id, std::string token,
          request_type type);
  Request(const Request &) = delete;
  Request(Request &&) = delete;
  Request operator=(const Request) = delete;
  ~Request() = default;
};

} // namespace transfer
} // namespace network
} // namespace db
} // namespace failless

#endif // LLSSDB_NETWORK_TRANSFER_REQUEST_H_
