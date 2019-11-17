#define BOOST_ASIO_HAS_CO_AWAIT
#include "llssdb/network/tcp_server.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/awaitable.hpp>

namespace failless {
namespace db {
namespace network {

void TcpServer::SetConfig(std::string ip, int port) {
    host_.ip = std::move(ip);
    host_.port = static_cast<unsigned short>(port);
}

void TcpServer::Listen() {
    boost::asio::io_context io_context(1);
//    unsigned short port = host_.port;
//    boost::asio::co_spawn(io_context, [&io_context, port](){});
//    boost::asio::co_spawn(io_context, [&io_context, port]() {
//            return listener(boost::asio::ip::tcp::acceptor(io_context, {boost::asio::ip::tcp::v4(), port})); },
//        boost::asio::detached);
    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait([&](auto, auto) { io_context.stop(); });
    io_context.run();
}

Host TcpServer::GetSettings() { return Host("0.0.0.0", 0); }

void TcpServer::SetResponseFunction(std::function<Response(Request &)> &generate_response) {}

void TcpServer::PushTask_(common::Task task) {}

TcpServer::TcpServer(Host host) : host_(std::move(host)) {}

TcpServer::TcpServer(std::string ip, int port) : host_(std::move(ip), port) {}

}  // namespace network
}  // namespace db
}  // namespace failless
