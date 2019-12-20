#include "llsscli/network/network_client.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <memory>
#include "llss3p/utils/queue.h"

namespace failless {
namespace client {
namespace network {

NetworkClient::NetworkClient(std::shared_ptr<config::NetworkConfig>& config) : config_(*config) {}

size_t NetworkClient::AddUserTask(
    std::shared_ptr<std::stringstream>& current_task,
    std::shared_ptr<std::function<size_t(char*)>>& callback) {
    // Init io_service & socket for one specific task
    std::shared_ptr<boost::asio::io_service> io_svc = std::make_shared<boost::asio::io_service>();
    std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(*io_svc);

    std::cout << current_task->str() << std::endl;
    // Init network-like task
    task_ = std::make_shared<config::NetworkConnectTask>(socket, io_svc, current_task, callback);
    OpenConnection();

    return 0;
}

size_t NetworkClient::OpenConnection() {
    std::cout << "Client is starting..." << std::endl;

    tcp::resolver Resolver(*task_->io_service.get());

    tcp::resolver::query Query(config_.db_host, config_.db_port);

    tcp::resolver::iterator EndPointIterator = Resolver.resolve(Query);
    tcp::endpoint end_point = *EndPointIterator;

    task_->socket->async_connect(
        end_point, boost::bind(&NetworkClient::OnConnect_, this, boost::asio::placeholders::error,
                               ++EndPointIterator, task_));

    std::cout << "Client is started!" << std::endl;

    task_->io_service->run();
//        boost::thread ClientThread(boost::bind(&boost::asio::io_service::run, task_->io_service));
//    //                network_client_->Close();
//    ClientThread.join();
    return 0;
}

size_t NetworkClient::Close() {
    task_->io_service->post(boost::bind(&NetworkClient::DoClose_, this, task_->socket));
    return 0;
}

void NetworkClient::OnConnect_(const boost::system::error_code& ErrorCode,
                               tcp::resolver::iterator EndPointIter,
                               std::shared_ptr<config::NetworkConnectTask>& task) {
    std::cout << "OnConnect..." << std::endl;

    if (ErrorCode.value() == boost::system::errc::success) {
        std::shared_ptr<std::string> str_task =
            std::shared_ptr<std::string>(new std::string(task->client_task->str()));

        std::cout << "Entered: " << task->client_task->str() << std::endl;

        task->socket->async_write_some(
            boost::asio::buffer(*str_task, str_task->length()),
            boost::bind(&NetworkClient::OnSend_, this, boost::asio::placeholders::error,
                        task->socket, task));

    } else if (EndPointIter != tcp::resolver::iterator()) {
        task->socket->close();
        tcp::endpoint EndPoint = *EndPointIter;

        task->socket->async_connect(
            EndPoint, boost::bind(&NetworkClient::OnConnect_, this,
                                  boost::asio::placeholders::error, ++EndPointIter, task));
    } else {
        throw std::logic_error("Can not connect to server");
    }
}

void NetworkClient::OnReceive_(const boost::system::error_code& ErrorCode,
                               std::shared_ptr<tcp::socket>& socket,
                               std::shared_ptr<config::NetworkConnectTask>& task) {
    std::cout << "receiving..." << std::endl;
    if (ErrorCode.value() == boost::system::errc::success) {
//        std::cout << content_buffer_vector_->data() << "<-data" << std::endl;
        std::cout << data << "<-data" << std::endl;

        task->client_callback->operator()(data);
        //        socket->async_read_some(boost::asio::buffer(recieve_buffer_, buflen_),
        //                                boost::bind(&NetworkClient::OnReceive_, this,
        //                                            boost::asio::placeholders::error, socket));
    } else {
        std::cout << "ERROR! OnReceive... with error = " << ErrorCode.value() << std::endl;
        DoClose_(socket);
    }
}

void NetworkClient::OnSend_(const boost::system::error_code& error_code,
                            std::shared_ptr<tcp::socket>& socket,
                            std::shared_ptr<config::NetworkConnectTask>& task) {
    std::cout << "sending..." << std::endl;
    if (!error_code) {
        std::cout << "\"" << task->client_task->str() << "\" has been sent" << std::endl;

        socket->async_read_some(boost::asio::buffer(data, max_length),
                              boost::bind(&NetworkClient::OnReceive_, this,
                                          boost::asio::placeholders::error, socket, task));
    } else {
        std::cout << "OnSend closing" << std::endl;
        DoClose_(socket);
    }
}

void NetworkClient::DoClose_(std::shared_ptr<tcp::socket>& socket) { socket->close(); }

}  // namespace network
}  // namespace client
}  // namespace failless
