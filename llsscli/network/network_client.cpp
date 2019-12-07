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
    : config_(*config) {
    content_buffer_vector_.resize(1024);
    user_socket_queue_ =
        std::make_shared<common::utils::Queue<std::shared_ptr<config::NetworkConnectTask>>>();
}

size_t NetworkClient::AddUserTask(std::shared_ptr<std::stringstream>& current_task,
                                  std::shared_ptr<std::function<size_t()>>& callback) {
    // Init io_service & socket for one specific task
    std::shared_ptr<boost::asio::io_service> io_svc = std::make_shared<boost::asio::io_service>();
    std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(*io_svc);

    // Init network-like task
    std::shared_ptr<config::NetworkConnectTask> task =
        std::make_shared<config::NetworkConnectTask>(socket, io_svc, current_task, callback);

    user_socket_queue_->Push(task);
    return 0;
}

size_t NetworkClient::OpenConnection() {
    while (!user_socket_queue_->IsEmpty()) {
        std::shared_ptr<config::NetworkConnectTask> temp = user_socket_queue_->Pop();

        std::cout << "Client is starting..." << std::endl;

        tcp::resolver Resolver(*temp->io_service.get());

        tcp::resolver::query Query(config_.db_host, config_.db_port);

        tcp::resolver::iterator EndPointIterator = Resolver.resolve(Query);
        tcp::endpoint end_point = *EndPointIterator;

        temp->socket->async_connect(
            end_point, boost::bind(&NetworkClient::OnConnect_, this,
                                   boost::asio::placeholders::error, ++EndPointIterator, temp));

        std::cout << "Client is started!" << std::endl;

        boost::thread ClientThread(
            boost::bind(&boost::asio::io_service::run, temp->io_service));

        //                network_client_->Close();
        ClientThread.join();
    }
    return 0;
}

size_t NetworkClient::Close() {
    while (!user_socket_queue_->IsEmpty()) {
        std::shared_ptr<config::NetworkConnectTask> temp = user_socket_queue_->Pop();
        temp->io_service->post(boost::bind(&NetworkClient::DoClose_, this, temp->socket));
    }
    return 0;
}

void NetworkClient::OnConnect_(const boost::system::error_code& ErrorCode,
                               tcp::resolver::iterator EndPointIter,
                               std::shared_ptr<config::NetworkConnectTask>& task) {
    std::cout << "OnConnect..." << std::endl;

    if (ErrorCode.value() == boost::system::errc::success) {
        std::shared_ptr<std::string> str_task =
            std::shared_ptr<std::string>(new std::string(task->client_task->str()));
        std::shared_ptr<std::string> str_task_len = std::shared_ptr<std::string>(
            new std::string(std::to_string(task->client_task->str().length())));

        std::cout << "Entered: " << *str_task_len << std::endl;

        task->socket->async_write_some(
            boost::asio::buffer(*str_task_len, str_task_len->length()),
            boost::bind(&NetworkClient::OnSend_, this, boost::asio::placeholders::error,
                        task->socket, str_task_len));

        std::cout << "Entered: " << task->client_task->str() << std::endl;

        task->socket->async_write_some(
            boost::asio::buffer(*str_task, str_task->length()),
            boost::bind(&NetworkClient::OnSend_, this, boost::asio::placeholders::error,
                        task->socket, str_task));

    } else if (EndPointIter != tcp::resolver::iterator()) {
        task->socket->close();
        tcp::endpoint EndPoint = *EndPointIter;

        task->socket->async_connect(
            EndPoint, boost::bind(&NetworkClient::OnConnect_, this,
                                  boost::asio::placeholders::error, ++EndPointIter, task));
    }
}

void NetworkClient::OnReceive_(const boost::system::error_code& ErrorCode,
                               std::shared_ptr<tcp::socket>& socket) {
    std::cout << "receiving..." << std::endl;
    if (ErrorCode.value() == boost::system::errc::success) {
        std::cout << content_buffer_vector_.data() << "<-data" << std::endl;

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

        socket->async_receive(boost::asio::buffer(content_buffer_vector_),
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
