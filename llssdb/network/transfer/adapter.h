#ifndef FAILLESS_LLSSDB_NETWORK_TRANSFER_ADAPTER_H_
#define FAILLESS_LLSSDB_NETWORK_TRANSFER_ADAPTER_H_

#include <boost/asio/io_service.hpp>
#include "llssdb/network/transfer/connection.h"

namespace failless {
namespace db {
namespace network {

struct ConnectionAdapter {
    ConnectionAdapter() : conn(nullptr) {}
    explicit ConnectionAdapter(boost::asio::io_service &io_service) {
        conn.reset(new Connection(io_service));
    }
    ~ConnectionAdapter() = default;
    std::shared_ptr<Connection> conn;
};

}  // namespace network
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_NETWORK_TRANSFER_ADAPTER_H_
