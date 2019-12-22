#include <memory>
#include <thread>
#include <unistd.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "llssdb/auth/iauthorization.h"
#include "llssdb/engine/server_manager.h"
#include "llssdb/network/tcp_server.h"
#include "llss3p/utils/boost_logger.h"
#include "llssdb/utils/config_manager.h"

void StartManager(std::shared_ptr<failless::db::engine::IServerManager> manager) {
    manager->Run();
}

int main(/*int argc, char **argv*/) {

    failless::db::utils::Settings settings;
    failless::db::utils::ConfigManager config_manager("../failless.conf");
    config_manager.Initialize(settings);
    failless::common::utils::BoostLogger::filter_logging(boost::log::trivial::debug);
    failless::common::utils::Queue<std::shared_ptr<failless::db::network::Connection>> queue;
    failless::db::auth::Authorization Auth;

    std::shared_ptr<failless::db::engine::IServerManager> manager(
        new failless::db::engine::ServerManager(queue));
    manager->SetSettings(settings);

    std::thread start_manager(StartManager, manager);
    std::unique_ptr<failless::db::network::ITcpServer> tcp_server(
        new failless::db::network::TcpServer(queue, "127.0.0.1", 11556));
    tcp_server->Listen();
    start_manager.join();
    /*
    // daemon code goes below
    int pid;
    pid = fork();
    if (pid == -1) // if error while forking child
    {
        std::cerr << "Error: Start Daemon failed" << std::endl;
        return -1;
    }
    if (!pid)
    {
        // this method gives our child all rights (write/read)
        umask(0);

        // creating a new session
        // so that child process (our daemon) won't depend on the parent process
        setsid();

        // this code removing inputs, outputs and errors from server daemon (now it's commented)
        // close(STDIN_FILENO);
        // close(STDOUT_FILENO);
        // close(STDERR_FILENO);


        std::thread start_manager(StartManager, manager);
        std::unique_ptr<failless::db::network::ITcpServer> tcp_server(
            new failless::db::network::TcpServer(queue, "127.0.0.1", 11556));
        tcp_server->Listen();
        start_manager.join();
        return 0;
    }
    else // closing parent
    {
        return 0;
    }
    */
}