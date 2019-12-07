#include "llsscli/network/network_client.h"
#include <boost/bind.hpp>
#include <iostream>

namespace failless {
namespace client {
namespace network {

NetworkClient::NetworkClient(boost::asio::io_service& io_service, config::NetworkConfig config)
    : io_service_(io_service), socket_(io_service), send_buffer_(""), config_(std::move(config)) {
    try {
        std::cout << "Client is starting..." << std::endl;

        tcp::resolver Resolver(io_service_);

        tcp::resolver::query Query(config_.db_host, config_.db_port);

        tcp::resolver::iterator EndPointIterator = Resolver.resolve(Query);
        tcp::endpoint end_point = *EndPointIterator;

        socket_.async_connect(end_point,
                              boost::bind(&NetworkClient::OnConnect_, this,
                                          boost::asio::placeholders::error, ++EndPointIterator));

        std::cout << "Client is started!" << std::endl;

        std::cout << "Enter a query string " << std::endl;

        std::thread ClientThread(boost::bind(&boost::asio::io_service::run, &io_service_));

        //        network_client_->Close();
        ClientThread.join();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "\nClosing";
}

void NetworkClient::Close() { io_service_.post(boost::bind(&NetworkClient::DoClose_, this)); }
void NetworkClient::OnConnect_(const boost::system::error_code& ErrorCode,
                               tcp::resolver::iterator EndPointIter) {
    std::cout << "OnConnect..." << std::endl;
    if (ErrorCode.value() == boost::system::errc::success) {
        std::cin >> send_buffer_;
        send_buffer_ += "\0";
        std::cout << "Entered: " << send_buffer_ << std::endl;

        socket_.async_write_some(
            boost::asio::buffer(send_buffer_, send_buffer_.length() + 1),
            boost::bind(&NetworkClient::OnSend_, this, boost::asio::placeholders::error));
    } else if (EndPointIter != tcp::resolver::iterator()) {
        socket_.close();
        tcp::endpoint EndPoint = *EndPointIter;

        socket_.async_connect(EndPoint,
                              boost::bind(&NetworkClient::OnConnect_, this,
                                          boost::asio::placeholders::error, ++EndPointIter));
    }
}

void NetworkClient::OnReceive_(const boost::system::error_code& ErrorCode) {
    std::cout << "receiving..." << std::endl;
    if (ErrorCode.value() == boost::system::errc::success) {
        std::cout << recieve_buffer_ << std::endl;

        //        socket_.async_read_some(boost::asio::buffer(recieve_buffer_, buflen_),
        //                               boost::bind(&NetworkClient::OnReceive_, this,
        //                               boost::asio::placeholders::error));
    } else {
        std::cout << "ERROR! OnReceive..." << std::endl;
        DoClose_();
    }
}

void NetworkClient::OnSend_(const boost::system::error_code& error_code) {
    std::cout << "sending..." << std::endl;
    if (!error_code) {
        std::cout << "\"" << send_buffer_ << "\" has been sent" << std::endl;
        send_buffer_ = "";

        socket_.async_receive(
            boost::asio::buffer(recieve_buffer_, buflen_),
            boost::bind(&NetworkClient::OnReceive_, this, boost::asio::placeholders::error));
    } else {
        std::cout << "OnSend closing" << std::endl;
        DoClose_();
    }
}

void NetworkClient::DoClose_() { socket_.close(); }

}  // namespace network
}  // namespace client
}  // namespace failless
