#include <thread>
#include "llssdb/engine/server_manager.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/network/tcp_server.h"
#include "llssdb/utils/config_manager.h"

void StartManager(std::shared_ptr<failless::db::engine::IServerManager> manager) { manager->Run(); }

int main(/*int argc, char **argv*/) {
    std::unique_ptr<failless::db::folder::FileSystemInterface> file_system(
        new failless::db::folder::FileSystem("../cmake-build-debug"));
    std::string kKey = "key";
    int8_t data[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
//    file_system->Set(kKey, data); TODO(rowbotman): fix it later

    //    std::shared_ptr<IDataWorker> data_worker(new DataWorker());
    //    data_worker->Create();
    failless::db::common::Settings settings;
    failless::db::utils::ConfigManager config_manager;
    config_manager.Initialize(settings);
    boost::lockfree::queue<failless::db::common::Task> input_queue_(
        1000);  // it is queue capacity. I don't like that and may be, because it is necessary for
                // lockfree::queue, we should write our custom thread safe queue
    std::shared_ptr<failless::db::engine::IServerManager> manager(
        new failless::db::engine::ServerManager(input_queue_));
    manager->SetSettings(settings);
    std::thread start_manager(StartManager, manager);
    std::unique_ptr<failless::db::network::ITcpServer> tcp_server =
        std::make_unique<failless::db::network::TcpServer>("127.0.0.1", 11556);
    tcp_server->Listen();
    start_manager.join();

//    failless::db::folder::TaskWorker tw("llssdb/CMakeFiles/llssdb.dir/storage/test_user");
//    int8_t value[3] = {1, 2, 3};
//    common::Task test_task(value, 3);
//    tw.AddTask(temp_task, failless::db::common::operators::SET);
//    tw.AddTask(temp_task, failless::db::common::operators::GET);

    return 0;
}