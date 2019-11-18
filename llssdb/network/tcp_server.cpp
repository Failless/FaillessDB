#define BOOST_ASIO_HAS_CO_AWAIT
#include "llssdb/network/tcp_server.h"
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/signal_set.hpp>

namespace failless {
namespace db {
namespace network {

void TcpServer::SetConfig(std::string ip, int port) {
    host_.ip = ip::address::from_string(ip);
    host_.port = static_cast<unsigned short>(port);
    // TODO(rowbotman): write tests on this function
    // i am not sure about it
    acceptor_.set_option(ip::tcp::acceptor::endpoint_type(host_.ip, host_.port));
}

void do_read() {
    async_read(sock_, buffer(read_buffer_), MEM_FN2(read_complete, _1, _2),
               MEM_FN2(on_read, _1, _2));
}
void do_write(const std::string &msg) {
    if (!started()) return;
    std::copy(msg.begin(), msg.end(), write_buffer_);
    sock_.async_write_some(buffer(write_buffer_, msg.size()), MEM_FN2(on_write, _1, _2));
}
size_t read_complete(const boost::system::error_code &err, size_t bytes) {
    // similar to the one shown in TCP Synchronous Client
}

void TcpServer::Listen() {
    is_run_ = true;
    boost::asio::io_context io_context(1);
    //    unsigned short port = host_.port;
    //    boost::asio::co_spawn(io_context, [&io_context, port](){});
    //    boost::asio::co_spawn(io_context, [&io_context, port]() {
    //            return listener(boost::asio::ip::tcp::acceptor(io_context,
    //            {boost::asio::ip::tcp::v4(), port})); },
    //        boost::asio::detached);
    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
    //    signals.async_wait([&](auto, auto) { io_context.stop(); });
    io_context.run();
}

Host TcpServer::GetSettings() { return host_; }

void TcpServer::SetResponseFunction(std::function<Response(Request &)> &generate_response) {
    generate_response_ = generate_response;
    acceptor_.async_accept(socket_,
                           std::bind(&generate_response_, this, boost::asio::placeholders::error,
                                     std::placeholders::_1));
}

void TcpServer::PushTask_(common::Task task) {}

TcpServer::TcpServer(Host host)
    : host_(std::move(host)),
      socket_(io_service_),
      acceptor_(io_service_, ip::tcp::endpoint(host_.ip, host_.port)) {}

TcpServer::TcpServer(std::string ip, unsigned short port)
    : host_(std::move(ip), port),
      socket_(io_service_),
      acceptor_(io_service_, ip::tcp::endpoint(ip::address::from_string(ip), port)) {}

TcpServer::TcpServer() {}

void TcpServer::Read_() {
    auto self(boost::shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
                            [this, self](boost::system::error_code ec, std::size_t length)
                            {
                              if (!ec)
                              {
                                  do_write(length);
                              }
                            });
    boost::asio::async_read(sock_, buffer(read_buffer_), MEM_FN2(read_complete, _1, _2),
                            MEM_FN2(on_read, _1, _2));
}
void TcpServer::Write_() {}

}  // namespace network
}  // namespace db
}  // namespace failless
