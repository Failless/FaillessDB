#include <boost/bind.hpp>
#include <memory>
#include <thread>
#include "llssdb/engine/server_manager.h"
#include "llssdb/network/tcp_server.h"
#include "llssdb/utils/config_manager.h"

void StartManager(std::shared_ptr<failless::db::engine::IServerManager> manager) { manager->Run(); }

int main(/*int argc, char **argv*/) {
    failless::db::utils::Settings settings;
    failless::db::utils::ConfigManager config_manager("../failless.conf");
    config_manager.Initialize(settings);
    failless::common::utils::Queue<int> queue;
    queue.Push(1);
    queue.Pop();
    // it is queue capacity. I don't like that and may be, because it is necessary for
    // lockfree::queue, we should write our custom thread safe queue
//    failless::common::utils::Queue<failless::common::utils::Task> task_queue(1000);
//    std::shared_ptr<failless::db::engine::IServerManager> manager(
//        new failless::db::engine::ServerManager(task_queue));
//    manager->SetSettings(settings);
//    std::thread start_manager(StartManager, manager);
//    std::unique_ptr<failless::db::network::ITcpServer> tcp_server =
//        std::make_unique<failless::db::network::TcpServer>("127.0.0.1", 11556);
//    tcp_server->Listen();
//    start_manager.join();
    return 0;
}

// ECHO SERVER TO DEBUG CLIENT IN client-network-test BRANCH
// importing libraries
//#include <boost/asio.hpp>
//#include <boost/bind.hpp>
//#include <boost/enable_shared_from_this.hpp>
//#include <iostream>
//
//using namespace boost::asio;
//using ip::tcp;
//using std::cout;
//using std::endl;
//
//class con_handler : public boost::enable_shared_from_this<con_handler> {
// private:
//    tcp::socket sock;
//    std::string message = "Hello From Server!";
//    enum { max_length = 1024 };
//    char data[max_length];
//
// public:
//    typedef boost::shared_ptr<con_handler> pointer;
//    con_handler(boost::asio::io_service& io_service) : sock(io_service) {}
//    // creating the pointer
//    static pointer create(boost::asio::io_service& io_service) {
//        return pointer(new con_handler(io_service));
//    }
//    // socket creation
//    tcp::socket& socket() { return sock; }
//
//    void start() {
//        sock.async_read_some(boost::asio::buffer(data, max_length),
//                             boost::bind(&con_handler::handle_read, shared_from_this(),
//                                         boost::asio::placeholders::error,
//                                         boost::asio::placeholders::bytes_transferred));
//
////        sock.async_write_some(boost::asio::buffer(message, max_length),
////                              boost::bind(&con_handler::handle_write, shared_from_this(),
////                                          boost::asio::placeholders::error,
////                                          boost::asio::placeholders::bytes_transferred));
//    }
//
//    void handle_read(const boost::system::error_code& err, size_t bytes_transferred) {
//        if (!err) {
//            cout << "Recieved data = " << data << endl;
//            sock.async_write_some(boost::asio::buffer(data, max_length),
//                                  boost::bind(&con_handler::handle_write, shared_from_this(),
//                                              boost::asio::placeholders::error,
//                                              boost::asio::placeholders::bytes_transferred));
//        } else {
//            std::cerr << "error here1: " << err.message() << std::endl;
//
//            sock.close();
//        }
//    }
//    void handle_write(const boost::system::error_code& err, size_t bytes_transferred) {
//        if (!err) {
//            cout << "Server sent Hello message! => " << bytes_transferred << endl;
//        } else {
//            std::cerr << "error here1: " << err.message() << endl;
//            sock.close();
//        }
//    }
//};
//
//class Server {
// private:
//    tcp::acceptor acceptor_;
//    boost::asio::io_service& io_service_;
//
//    void start_accept() {
//        // socket
//        con_handler::pointer connection = con_handler::create(io_service_);
//
//        // asynchronous accept operation and wait for a new connection.
//        acceptor_.async_accept(connection->socket(),
//                               boost::bind(&Server::handle_accept, this, connection,
//                                           boost::asio::placeholders::error));
//    }
//
// public:
//    // constructor for accepting connection from client
//    Server(boost::asio::io_service& io_service)
//        : acceptor_(io_service, tcp::endpoint(ip::address::from_string("127.0.0.1"), 11564)),
//          io_service_(io_service) {
//        start_accept();
//    }
//    void handle_accept(con_handler::pointer connection, const boost::system::error_code& err) {
//        if (!err) {
//            connection->start();
//        }
//        start_accept();
//    }
//};
//
//int main(int argc, char* argv[]) {
//    try {
//        boost::asio::io_service io_service;
//        Server server(io_service);
//        io_service.run();
//    } catch (std::exception& e) {
//        std::cerr << e.what() << endl;
//    }
//}