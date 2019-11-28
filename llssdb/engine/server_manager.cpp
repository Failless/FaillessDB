#include "llssdb/engine/server_manager.h"
#include <llssdb/network/tcp_server.h>
#include <iostream>
#include <thread>

namespace failless {
namespace db {
namespace engine {

void ServerManager::SetTask(common::Task task) {}

void ServerManager::Reload() {}

void ServerManager::Run() {
    is_run_ = true;
    const int kMsDelay = 100;  // it's temporary
//    common::Task task{};
    network::ConnectionAdapter connection;
    while (is_run_) {
        if (task_queue_.empty()) {
            std::this_thread::sleep_for(std::chrono::microseconds(kMsDelay));
            continue;  // May by it will be better   if I change if-body to all code below here
        }
        task_queue_.pop(connection);
        switch (connection.command) {
            case common::operators::CREATE: {
                int folder_num = CreateFolder_(connection.client_id);
                std::cout << "Created new folder with id " << folder_num << std::endl;
                break;
            }
            case common::operators::KILL: {
                KillFolder_(connection.payload.folder_id);
                break;
            }
            case common::operators::CONNECT: {
                // create connection_worker for exist folder in the other thread
                break;
            }
            case common::operators::DISCONNECT: {
                // join tread with exist connection_worker which have created data dump before
                break;
            }
            default: {
                Execute_(connection);
                break;
            }
        }
    }
}

void ServerManager::Stop() { is_run_ = false; }

bool ServerManager::Execute_(common::Task& task) { return false; }

int ServerManager::CreateFolder_(boost::uuids::uuid& client_id) { return false; }

bool ServerManager::KillFolder_(int folder_id) { return false; }

bool ServerManager::RedirectTask_(common::Task& task) { return false; }

common::operators ServerManager::HandleRequest_(common::Task& Task) { return common::DELETE; }

void ServerManager::SetSettings(common::Settings& settings) {}

}  // namespace engine
}  // namespace db
}  // namespace failless
