#include "llssdb/network/connection.h"
#include <boost/asio/write.hpp>
#include <iostream>

namespace failless {
namespace db {
namespace network {

Connection::Connection(ip::tcp::socket socket) : socket_(std::move(socket)) {}
void Connection::Read() {
    data_ = std::make_unique<std::vector<std::byte>>(kMaxSize);
    // Begin an asynchronous socket read, upon completion invoke
    // the lambda function specified
    socket_.async_read_some(
        boost::asio::buffer(data_->data(), data_->size()),
        // Retain lifetime of the i/o buffer until completion
        [this, buffer = std::move(data_)](const boost::system::error_code &ec, size_t bytes) {
            // Handle the buffer read
            if (ec) {
                std::cerr << "Buffer read failed with " << ec << std::endl;
                return;
            }
            std::cout << "Read " << bytes << " bytes into buffer" << std::endl;
            Write(bytes);
        });
    // buffer will be dynamically freed now
    //    socket_.async_read_some(boost::asio::buffer(data_, max_length_),
    //                            [this, self](boost::system::error_code ec, std::size_t length) {
    //                                if (!ec) {
    //                                    do_write(length);
    //                                }
    //                            });
}
void Connection::Write(size_t size) {
    boost::asio::async_write(
        socket_, boost::asio::buffer(data_->data(), size),
        [this, buffer = std::move(data_)](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                Read();
            }
        });
}

}  // namespace network
}  // namespace db
}  // namespace failless
