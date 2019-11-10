#include <utility>
#include "llssdb/network/tcp_server.h"
#include "llssdb/engine/task.h"

namespace failless {
namespace db {
namespace network {

void TcpServer::SetConfig(std::string ip, int port) {}

void TcpServer::Listen() {}

Host TcpServer::GetSettings() { return Host("0.0.0.0", 0); }

void TcpServer::SetResponseFunction(
    std::function<transfer::Response(transfer::Request &)> &generate_response) {
}

void TcpServer::PushTask_(engine::Task task) {}

TcpServer::TcpServer(Host host) : host_(std::move(host)) {}

TcpServer::TcpServer(std::string ip, int port) : host_(std::move(ip), port) {}

} // namespace network
} // namespace db
} // namespace failless
