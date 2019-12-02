#include "llssdb/network/connection.h"
#include <llss3p/serialization/serializer.h>
#include <llss3p/serialization/serializer_interface.h>
#include <boost/asio.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>

namespace failless {
namespace db {
namespace network {

namespace srz = common::serializer;
namespace utl = common::utils;

Connection::Connection(boost::asio::io_service& io_service) : socket_(io_service) {}

void Connection::Read(const boost::system::error_code& err, size_t bytes_transferred) {
    if (!err) {
        std::cout << buffer_ << std::endl;
        std::unique_ptr<srz::SerializerInterface<utl::Packet>> serializer(
            new srz::Serializer<utl::Packet>);
        serializer->Deserialize();
        has_ = true;
    } else {
        std::cerr << "error: " << err.message() << std::endl;
        socket_.close();
    }
}

void Connection::Write(const boost::system::error_code& err, size_t bytes_transferred) {
    if (!err) {
        std::cout << "Server sent Hello message!" << std::endl;
    } else {
        std::cerr << "error: " << err.message() << std::endl;
        socket_.close();
    }
}

ip::tcp::socket& Connection::GetSocket() { return socket_; }

void Connection::DoJob() {
    socket_.async_read_some(boost::asio::buffer(buffer_, kMaxSize),
                            boost::bind(&Connection::Read, this, boost::asio::placeholders::error,
                                        boost::asio::placeholders::bytes_transferred));
}
void Connection::SendData(common::utils::Packet data) {
    socket_.async_write_some(boost::asio::buffer(buffer_, message.size()),
                             boost::bind(&Connection::Write, this, boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}
bool Connection::HasData() const { return has_; }

bool Connection::GetData(utils::Task& task) {
    if (!has_) return false;
}

}  // namespace network
}  // namespace db
}  // namespace failless
