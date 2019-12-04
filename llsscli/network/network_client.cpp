#include "llsscli/network/network_client.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <memory>
#include "llss3p/utils/queue.h"

namespace failless {
namespace client {
namespace network {

NetworkClient::NetworkClient(std::shared_ptr<config::NetworkConfig>& config)
    : config_(config.operator*()) {}

size_t NetworkClient::AddUserTask(std::shared_ptr<std::stringstream>& current_task,
                                  std::shared_ptr<std::function<size_t()>>& callback) {
    std::shared_ptr<boost::asio::io_service> io_svc = std::make_shared<boost::asio::io_service>();
    std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(*io_svc);
    std::shared_ptr<config::NetworkConnectTask> task =
        std::make_shared<config::NetworkConnectTask>(socket, io_svc, current_task, callback);

    user_socket_queue_ =
        std::make_shared<common::utils::Queue<std::shared_ptr<config::NetworkConnectTask>>>();
    user_socket_queue_->Push(task);
    return 0;
}

size_t NetworkClient::OpenConnection() {
    while (!user_socket_queue_.operator*().IsEmpty()) {
        std::shared_ptr<config::NetworkConnectTask> temp = user_socket_queue_->Pop();

        std::cout << "Client is starting... with data " << temp->client_task->str() << std::endl;

        tcp::resolver Resolver(*temp->io_service.get());

        tcp::resolver::query Query(config_.db_host, config_.db_port);

        tcp::resolver::iterator EndPointIterator = Resolver.resolve(Query);
        tcp::endpoint end_point = *EndPointIterator;

        temp->socket->async_connect(
            end_point,
            boost::bind(&NetworkClient::OnConnect_, this, boost::asio::placeholders::error,
                        ++EndPointIterator, temp->socket, temp->client_task));

        std::cout << "Client is started!" << std::endl;

        boost::thread ClientThread(
            boost::bind(&boost::asio::io_service::run, temp.get()->io_service));

        //                network_client_->Close();
        ClientThread.join();
    }
    return 0;
}

size_t NetworkClient::Close() {
    while (!user_socket_queue_.operator*().IsEmpty()) {
        std::shared_ptr<config::NetworkConnectTask> temp = user_socket_queue_->Pop();

        temp->io_service->post(boost::bind(&NetworkClient::DoClose_, this, temp->socket));
    }
    return 0;
}

void NetworkClient::OnConnect_(const boost::system::error_code& ErrorCode,
                               tcp::resolver::iterator EndPointIter,
                               std::shared_ptr<tcp::socket>& socket,
                               std::shared_ptr<std::stringstream>& current_task) {
    std::cout << "OnConnect..." << std::endl;
    if (ErrorCode.value() == boost::system::errc::success) {
        std::shared_ptr<std::string> str_task(new std::string(current_task->str()));
        std::cout << "Entered: " << current_task->str() << std::endl;
        socket->async_write_some(boost::asio::buffer(current_task->str()),
                                 boost::bind(&NetworkClient::OnSend_, this,
                                             boost::asio::placeholders::error, socket, str_task));
    } else if (EndPointIter != tcp::resolver::iterator()) {
        socket->close();
        tcp::endpoint EndPoint = *EndPointIter;

        socket->async_connect(EndPoint, boost::bind(&NetworkClient::OnConnect_, this,
                                                    boost::asio::placeholders::error,
                                                    ++EndPointIter, socket, current_task));
    }
}

void NetworkClient::OnReceive_(const boost::system::error_code& ErrorCode,
                               std::shared_ptr<tcp::socket>& socket) {
    std::cout << "receiving..." << std::endl;
    if (ErrorCode.value() == boost::system::errc::success) {
        std::cout << recieve_buffer_ << std::endl;

//        socket->async_read_some(boost::asio::buffer(recieve_buffer_, buflen_),
//                                boost::bind(&NetworkClient::OnReceive_, this,
//                                            boost::asio::placeholders::error, socket));
    } else {
        std::cout << "ERROR! OnReceive..." << std::endl;
        DoClose_(socket);
    }
}

void NetworkClient::OnSend_(const boost::system::error_code& error_code,
                            std::shared_ptr<tcp::socket>& socket,
                            std::shared_ptr<std::string>& str_task) {
    std::cout << "sending..." << std::endl;
    if (!error_code) {
        std::cout << "\"" << *str_task << "\" has been sent" << std::endl;

        socket->async_receive(boost::asio::buffer(recieve_buffer_, buflen_),
                              boost::bind(&NetworkClient::OnReceive_, this,
                                          boost::asio::placeholders::error, socket));
    } else {
        std::cout << "OnSend closing" << std::endl;
        DoClose_(socket);
    }
}

void NetworkClient::DoClose_(std::shared_ptr<tcp::socket>& socket) { socket->close(); }

}  // namespace network
}  // namespace client
}  // namespace failless
