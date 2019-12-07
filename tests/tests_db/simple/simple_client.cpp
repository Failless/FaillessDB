#include "tests/tests_db/simple/simple_client.h"
#include <boost/asio.hpp>
#include <iostream>
#include <utility>

namespace failless {
namespace db {
namespace tests {

std::string SimpleClient::Ping() {
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket(io_service);
    socket.connect(
        boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 11556));
    const std::string msg = "Hello From Client!\n";
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(msg), error);
    if (!error) {
        std::cout << "Client sent hello message!" << std::endl;
    } else {
        std::cout << "send failed: " << error.message() << std::endl;
    }
    boost::asio::streambuf receive_buffer;
    boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
    if (error && error != boost::asio::error::eof) {
        std::cout << "receive failed: " << error.message() << std::endl;
        return std::string("");
    } else {
//        auto data = boost::asio::buffer_cast<std::string>(receive_buffer.data());
//        std::cout << data << std::endl;
        return "data";
    }
}

void SimpleClient::SetConfig(network::Host host) { host_ = std::move(host); }

void SimpleClient::SetConfig(std::string ip, int port) {
    host_ = network::Host(std::move(ip), port);
}

}  // namespace tests
}  // namespace db
}  // namespace failless
