#include "llssdb/network/tcp_server.h"

void failless::db::network::TcpServer::setConfig(std::string ip, int port) {}

void failless::db::network::TcpServer::listen() {}

failless::db::network::Host failless::db::network::TcpServer::getSettings() {
  return failless::db::network::Host("0.0.0.0", 0);
}

void failless::db::network::TcpServer::setResponseFunction(
    std::function<transfer::Response(transfer::Request &)> &generate_response) {
}
