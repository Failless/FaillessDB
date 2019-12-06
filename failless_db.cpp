#include <boost/bind.hpp>
#include <memory>
#include <thread>
#include "llssdb/engine/server_manager.h"
#include "llssdb/network/tcp_server.h"
#include "llssdb/utils/config_manager.h"

void StartManager(const std::shared_ptr<failless::db::engine::IServerManager>& manager) {
    manager->Run();
}

int main(/*int argc, char **argv*/) {
    failless::db::utils::Settings settings;
    failless::db::utils::ConfigManager config_manager("../failless.conf");
    config_manager.Initialize(settings);
    failless::common::utils::Queue<std::shared_ptr<failless::db::network::Connection>> queue;
    std::shared_ptr<failless::db::engine::IServerManager> manager(
        new failless::db::engine::ServerManager(queue));
    manager->SetSettings(settings);
    std::thread start_manager(StartManager, manager);
    std::unique_ptr<failless::db::network::ITcpServer> tcp_server(
        new failless::db::network::TcpServer(queue, "127.0.0.1", 11556));
    tcp_server->Listen();
    start_manager.join();
    return 0;
}
