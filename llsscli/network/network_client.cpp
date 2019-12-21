#include "llsscli/network/network_client.h"
#include <llss3p/serialization/serializer.h>
#include <llss3p/serialization/serializer_interface.h>
#include <llss3p/utils/packet.h>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <memory>
#include "llss3p/utils/queue.h"

namespace failless {
namespace client {
namespace network {

NetworkClient::NetworkClient(std::shared_ptr<config::NetworkConfig>& config) : config_(*config) {}

size_t NetworkClient::AddUserTask(std::shared_ptr<std::stringstream>& current_task,
                                  std::shared_ptr<std::function<size_t(char*, size_t)>>& callback) {
    // Init io_service & socket for one specific task
    std::shared_ptr<boost::asio::io_service> io_svc = std::make_shared<boost::asio::io_service>();
    std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(*io_svc);

    // Init network-like task
    task_ = std::make_shared<config::NetworkConnectTask>(socket, io_svc, current_task, callback);
    OpenConnection();

    return 0;
}

size_t NetworkClient::OpenConnection() {
    std::cout << "[NETWORK] Client is starting..." << std::endl;

    tcp::resolver Resolver(*task_->io_service);

    tcp::resolver::query Query(config_.db_host, config_.db_port);

    tcp::resolver::iterator EndPointIterator = Resolver.resolve(Query);
    tcp::endpoint end_point = *EndPointIterator;

    task_->socket->async_connect(
        end_point, boost::bind(&NetworkClient::OnConnect_, this, boost::asio::placeholders::error,
                               ++EndPointIterator, task_));

    std::cout << "[NETWORK] Client is started!" << std::endl;

    task_->io_service->run();
    return 0;
}

size_t NetworkClient::Close() {
    task_->io_service->post(boost::bind(&NetworkClient::DoClose_, this, task_->socket));
    return 0;
}

void NetworkClient::OnConnect_(const boost::system::error_code& ErrorCode,
                               tcp::resolver::iterator EndPointIter,
                               std::shared_ptr<config::NetworkConnectTask>& task) {
    std::cout << "[NETWORK] OnConnect..." << std::endl;

    if (ErrorCode.value() == boost::system::errc::success) {
        std::shared_ptr<std::string> str_task =
            std::make_shared<std::string>(task->client_task->str());

        std::cout << "[NETWORK] Entered and serialized: " << task->client_task->str() << std::endl;

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
        throw std::logic_error("[NETWORK] Can not connect to server");
    }
}

void NetworkClient::OnReceive_(const boost::system::error_code& ErrorCode, size_t bytes_transferred,
                               std::shared_ptr<tcp::socket>& socket,
                               std::shared_ptr<config::NetworkConnectTask>& task) {
    std::cout << "[NETWORK] receiving..." << std::endl;
    if (ErrorCode.value() == boost::system::errc::success) {
        std::cout << "[NETWORK] Serialized data received\""<< data << "\"" << std::endl;
        task->client_callback->operator()(data, bytes_transferred);
    } else {
        std::cerr << "[NETWORK] ERROR! OnReceive... with error = " << ErrorCode.value()
                  << std::endl;
        std::cerr << ErrorCode.message() << std::endl;

        DoClose_(socket);
    }
}

void NetworkClient::OnSend_(const boost::system::error_code& error_code,
                            std::shared_ptr<tcp::socket>& socket,
                            std::shared_ptr<config::NetworkConnectTask>& task) {
    std::cout << "[NETWORK] sending..." << std::endl;
    if (!error_code) {
        std::cout << "[NETWORK] \"" << task->client_task->str() << "\" has been sent" << std::endl;

        socket->async_read_some(
            boost::asio::buffer(data, max_length),
            boost::bind(&NetworkClient::OnReceive_, this, boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred, socket, task));
    } else {
        std::cout << "[NETWORK] OnSend closing" << std::endl;
        DoClose_(socket);
    }
}

void NetworkClient::DoClose_(std::shared_ptr<tcp::socket>& socket) { socket->close(); }

}  // namespace network
}  // namespace client
}  // namespace failless
