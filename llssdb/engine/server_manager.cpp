#include "llssdb/engine/server_manager.h"
#include <thread>

namespace failless {
namespace db {
namespace engine {

void ServerManager::SetTask(utils::Task task) {}

void ServerManager::Reload() {}

void ServerManager::Run() {
    is_run_ = true;
    const int kMsDelay = 100;  // it's temporary
                               //    utils::Task task{};
    while (is_run_) {
        if (task_queue_.IsEmpty()) {
            std::this_thread::sleep_for(std::chrono::microseconds(kMsDelay));
            continue;  // May by it will be better   if I change if-body to all code below here
        }
        network::ConnectionAdapter connection = task_queue_.Pop();
        //        switch (connection.conn) {
        //            case common::enums::operators::CREATE: {
        //                int folder_num = CreateFolder_(connection.client_id);
        //                std::cout << "Created new folder with id " << folder_num << std::endl;
        //                break;
        //            }
        //            case common::enums::operators::KILL: {
        //                KillFolder_(connection.payload.folder_id);
        //                break;
        //            }
        //            case common::enums::operators::CONNECT: {
        //                // create connection_worker for exist folder in the other thread
        //                break;
        //            }
        //            case common::enums::operators::DISCONNECT: {
        //                // join tread with exist connection_worker which have created data dump
        //                before break;
        //            }
        //            default: {
        //                Execute_(connection);
        //                break;
        //            }
        //        }
    }
}

void ServerManager::Stop() { is_run_ = false; }

bool ServerManager::Execute_(utils::Task& task) { return false; }

int ServerManager::CreateFolder_(boost::uuids::uuid& client_id) { return false; }

bool ServerManager::KillFolder_(int folder_id) { return false; }

bool ServerManager::RedirectTask_(utils::Task& task) { return false; }

common::enums::operators ServerManager::HandleRequest_(utils::Task& Task) {
    return common::enums::DELETE;
}

void ServerManager::SetSettings(utils::Settings& settings) {}

}  // namespace engine
}  // namespace db
}  // namespace failless
