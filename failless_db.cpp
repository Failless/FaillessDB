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