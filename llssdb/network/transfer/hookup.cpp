#include "llssdb/network/transfer/hookup.h"
#include <boost/asio.hpp>
#include <boost/asio/write.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include "llss3p/serialization/serializer.h"

namespace failless {
namespace db {
namespace network {

namespace srz = common::serializer;
namespace utl = common::utils;

Connection::Connection(boost::asio::io_service& io_service) : socket_(io_service) {}

void Connection::Read(const boost::system::error_code& err, size_t bytes_transferred) {
    if (!err) {
        input_buffer_.commit(bytes_transferred);
        std::cout << input_buffer_.size() << std::endl;
        //        boost::asio::streambuf::const_buffers_type buf = asio_buf_.data();
        //        std::string str(boost::asio::buffers_begin(buf),
        //                        boost::asio::buffers_begin(buf) + asio_buf_.size());
        std::unique_ptr<srz::SerializerInterface<utl::Packet>> serializer(
            new srz::Serializer<utl::Packet>);
        try {
            std::string str(boost::asio::buffers_begin(input_buffer_.data()),
                            boost::asio::buffers_begin(input_buffer_.data()) + bytes_transferred);
            packet_ = serializer->Deserialize(const_cast<char*>(str.c_str()),
                                              bytes_transferred);
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
        input_buffer_.consume(bytes_transferred);
        std::cout << "Server sent Hello message!" << std::endl;
    } else {
        std::cerr << "error: " << err.message() << std::endl;
        socket_.close();
    }
}

ip::tcp::socket& Connection::GetSocket() { return socket_; }

void Connection::DoJob() {
    //    socket_.async_read_some(boost::asio::buffer(buffer_, kMaxSize),
    //                            boost::bind(&Connection::Read_, this,
    //                            boost::asio::placeholders::error,
    //                                        boost::asio::placeholders::bytes_transferred));
    const char* kTestVesion = std::getenv("LLSS_PART");
    if (kTestVesion) {
//        input_buffer_.resize(256);
//        socket_.async_read_some(
//            boost::asio::dynamic_buffer(input_buffer_, 256),
//            boost::bind(&Connection::Read, this, boost::asio::placeholders::error,
//                        boost::asio::placeholders::bytes_transferred));
    } else {
//        input_buffer_.resize(kMaxSize);
        socket_.async_read_some(
            input_buffer_.prepare(kMaxSize),
            boost::bind(&Connection::Read, this, boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }
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
    task.folder_id = packet_.data.folder_id;
    return true;
}

common::utils::Packet* Connection::GetPacket() {
    if (!has_) return nullptr;
    return &packet_;
}

}  // namespace network
}  // namespace db
}  // namespace failless