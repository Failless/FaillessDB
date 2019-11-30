#include "llssdb/network/tcp_server.h"
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <utility>

namespace failless {
namespace db {
namespace network {

void TcpServer::SetConfig(std::string ip, int port) {
    host_.ip = ip::address::from_string(ip);
    host_.port = static_cast<unsigned short>(port);
    try {
        if (acceptor_->is_open()) {
            acceptor_->cancel();
            acceptor_->close();
        }
        // TODO(rowbotman): write tests on this function
        // i am not sure about it
        acceptor_->bind(ip::tcp::acceptor::endpoint_type(host_.ip, host_.port));
        acceptor_->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    } catch (const boost::system::system_error &error) {
        std::cerr << "Error accepting" << std::endl;
    }
}

void TcpServer::Listen() {
    is_run_ = true;
    //    boost::asio::io_context io_context(1);
    //    boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
    //    signals.async_wait([&](auto, auto) { io_context.stop(); });
    io_service_.run();
}

Host TcpServer::GetSettings() { return host_; }

void TcpServer::SetResponseFunction(std::function<Response(Request &)> &generate_response) {
    // TODO(rowbotman) : change understand code and rewrite this part
    // generate_response_ = generate_response;
    //    connections_.emplace_back(Connection(io_service_));
    //    acceptor_->async_accept(socket);
    //    acceptor_->async_accept(*socket_, AcceptHandler_);
    //                           std::bind(/*&generate_response_*/, this,
    //                           boost::asio::placeholders::error,
    //                                     std::placeholders::_1));
}

void TcpServer::PushTask_(utils::Task task) {}

TcpServer::TcpServer(Host host) : host_(std::move(host)) {
    acceptor_ =
        std::make_unique<ip::tcp::acceptor>(io_service_, ip::tcp::endpoint(host_.ip, host_.port));
}

TcpServer::TcpServer(std::string ip, unsigned short port) : host_(std::move(ip), port) {
    acceptor_ =
        std::make_unique<ip::tcp::acceptor>(io_service_, ip::tcp::endpoint(host_.ip, host_.port));
    Accept_();
}

void TcpServer::Accept_() {
    ConnectionAdapter adapter(io_service_);
    acceptor_->async_accept(
        adapter.conn->GetSocket(),
        boost::bind(&TcpServer::AcceptHandler_, this, adapter, boost::asio::placeholders::error));
}

void TcpServer::AcceptHandler_(ConnectionAdapter adaptor, const boost::system::error_code &error) {
    if (!error) {
        adaptor.conn->DoJob();
    }
    Accept_();
}

}  // namespace network
}  // namespace db
}  // namespace failless
