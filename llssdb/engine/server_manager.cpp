#include "llssdb/engine/server_manager.h"
#include <iostream>
#include <thread>

namespace failless {
namespace db {
namespace engine {

void ServerManager::SetTask(common::Task task) {}

void ServerManager::Reload() {}

//void ServerManager::Run() {
//    is_run_ = true;
//    const int kMsDelay = 100;  // it's temporary
//    common::Task task{};
//    while (is_run_) {
//        if (input_queue_.empty()) {
//            std::this_thread::sleep_for(std::chrono::microseconds(kMsDelay));
//            continue;  // May by it will be better if I change if-body to all code below here
//        }
//        input_queue_.pop(task);
//        switch (task.command) {
//            case common::operators::CREATE: {
//                int folder_num = CreateFolder_(task.client_id);
//                std::cout << "Created new folder with id " << folder_num << std::endl;
//                break;
//            }
//            case common::operators::KILL: {
//                KillFolder_(task.payload.folder_id);
//                break;
//            }
//            case common::operators::CONNECT: {
//                // create task_worker for exist folder in the other thread
//                break;
//            }
//            case common::operators::DISCONNECT: {
//                // join tread with exist task_worker which have created data dump before
//                break;
//            }
//            default: {
//                Execute_(task);
//                break;
//            }
//        }
//    }
//}

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
