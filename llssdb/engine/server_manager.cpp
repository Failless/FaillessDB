#include "llssdb/engine/server_manager.h"

namespace failless {
namespace db {
namespace engine {

void ServerManager::SetTask(common::Task task) {}

void ServerManager::Reload() {}

void ServerManager::Run() {
    while (is_run_) {
//        if (!task_queue_.empty()) {
//            auto task = task_queue_.pop();
//        }
    }
}

void ServerManager::Stop() {}

bool ServerManager::Execute_(common::operators command) { return false; }

bool ServerManager::CreateFolder_() { return false; }

bool ServerManager::KillFolder_(int folder_id) { return false; }

bool ServerManager::RedirectTask_(common::Task& task) { return false; }

common::operators ServerManager::HandleRequest_(common::Task& Task) { return common::DELETE; }

void ServerManager::SetSettings(common::Settings& settings) {}

}  // namespace engine
}  // namespace db
}  // namespace failless
