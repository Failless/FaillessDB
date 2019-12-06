#include "llssdb/engine/server_manager.h"
#include <iostream>
#include <thread>

namespace failless {
namespace db {
namespace engine {

void ServerManager::SetTask(utils::Task task) {}

void ServerManager::Reload() {}

void ServerManager::Run() {
    is_run_ = true;
    const int kMsDelay = 100;  // it's temporary
    while (is_run_) {
        if (task_queue_.IsEmpty()) {
            std::this_thread::sleep_for(std::chrono::microseconds(kMsDelay));
            continue;  // May by it will be better   if I change if-body to all code below here
        }
        auto connection = task_queue_.Pop();
        while (!connection->HasData()) {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
        utils::ServerTask task;
        connection->GetData(task);
        if (task.command == common::enums::operators::REG) {
            if (users_->Registration(task.login, task.password)) {
                common::utils::Packet packet;
                packet.ret_value = common::enums::response_type::OK;
                connection->SendData(packet);
                continue;
            }
        }
        switch (task.command) {
            case common::enums::operators::CREATE: {
                std::cout << "Created new folder with id " << std::endl;
                break;
            }
            case common::enums::operators::KILL: {
                break;
            }
            case common::enums::operators::CONNECT: {
                // create connection_worker for exist folder in the other thread
                break;
            }
            case common::enums::operators::DISCONNECT: {
                // join tread with exist connection_worker which have created data dump before
                break;
            }
            default: {
//                Execute_(connection);
                break;
            }
        }
    }
}  // namespace engine

void ServerManager::Stop() { is_run_ = false; }

bool ServerManager::Execute_(utils::Task& task) { return false; }

int ServerManager::CreateFolder_(boost::uuids::uuid& client_id) { return false; }

bool ServerManager::KillFolder_(int folder_id) { return false; }

bool ServerManager::RedirectTask_(utils::Task& task) { return false; }

common::enums::operators ServerManager::HandleRequest_(utils::Task& Task) {
    return common::enums::DELETE;
}

void ServerManager::SetSettings(utils::Settings& settings) {}

ServerManager::ServerManager(common::utils::Queue<std::shared_ptr<network::Connection>>& task_queue)
    : task_queue_(task_queue),
      //          folders_(0),  // I don't think that it's necessary but...
      is_run_(false) {
    users_ = std::make_unique<auth::Authorization>();
}

}  // namespace engine
}  // namespace db
}  // namespace failless