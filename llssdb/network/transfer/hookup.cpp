#include "llssdb/network/transfer/hookup.h"
#include "llss3p/serialization/serializer.h"
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
        for (int i = 0; i < bytes_transferred; ++i) {
            std::cout << buffer_[i];
        }
        std::cout << std::endl;
        std::unique_ptr<srz::SerializerInterface<utl::Packet>> serializer(
            new srz::Serializer<utl::Packet>);
        try {
            packet_ = serializer->Deserialize(buffer_, bytes_transferred);
            has_ = true;
        } catch (std::bad_cast& e) {
            std::cerr << e.what() << std::endl;
            socket_.close();
        }
    } else {
        std::cerr << "error: " << err.message() << std::endl;
        socket_.close();
    }
}

void Connection::Write(const boost::system::error_code& err, size_t bytes_transferred) {
    has_ = false;
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
void Connection::SendData(common::utils::Packet& data) {
    std::unique_ptr<srz::SerializerInterface<common::utils::Packet>> serializer(
        new srz::Serializer<common::utils::Packet>());
    serializer->Serialize(data);
    auto ser_data = serializer->GetOutStringStream();
    ser_data->seekg(0, std::ios::end);
    socket_.async_write_some(boost::asio::buffer(ser_data->str(), ser_data->tellg()),
                             boost::bind(&Connection::Write, this, boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}
bool Connection::HasData() const { return has_; }

bool Connection::GetMetaData(utils::ServerTask& task) {
    if (!has_) return false;
    task.command = static_cast<common::enums::operators>(packet_.command);
    task.login = &packet_.login;
    task.password = &packet_.pass;
    return true;
}

common::utils::Packet* Connection::GetPacket() {
    if (!has_) return nullptr;
    return &packet_;
}

}  // namespace network
}  // namespace db
}  // namespace failless