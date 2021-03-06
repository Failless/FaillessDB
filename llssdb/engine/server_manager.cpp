#include "llssdb/engine/server_manager.h"

#include <thread>

#include <boost/log/trivial.hpp>

namespace failless {
namespace db {
namespace engine {

void WorkInThread(common::utils::Queue<std::shared_ptr<network::Connection>>* queue,
                  const utils::WorkerSettings& settings,
                  std::string login) {
    BOOST_LOG_TRIVIAL(info) << "[SM]: Starting new thread for TaskWorker";
    std::unique_ptr<folder::ITaskWorker> worker(
        new folder::TaskWorker(*queue, settings.db_path + "/storage/" + login, settings.do_backup));
    worker->Work();
}

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
        utils::ServerTask task{};
        connection->GetMetaData(task);
        common::utils::Packet packet;
        if (task.command == common::enums::operators::REG) {
            BOOST_LOG_TRIVIAL(debug) << "[SM]: Received command REG";
            packet.ret_value = common::enums::response_type::OK;
            packet.command = common::enums::operators::REG;
            if (users_->Registration(*task.login, *task.password, 0)) {
                packet.ret_value = common::enums::response_type::OK;
            } else {
                packet.ret_value = common::enums::response_type::SERVER_ERROR;
            }
            connection->SendData(packet);
            continue;
        }

        //        if (!users_->IsAuth(*task.login, *task.password, 0)) {
        //            packet.ret_value = common::enums::response_type::NOT_ALLOWED;
        //            connection->SendData(packet);
        //            continue;
        //        }

        switch (task.command) {
            case common::enums::operators::CREATE: {
                BOOST_LOG_TRIVIAL(debug) << "[SM]: Received command CREATE";
                //                common::utils::Queue<std::shared_ptr<network::Connection>>
                //                folder_queue;
                if (!task.folder_id || task.folder_id > 15) {
                    task.folder_id = FindEmpty_();
                    if (task.folder_id == -1) {
                        //                    folders_.emplace_back();
                        task.folder_id = static_cast<short>(folders_.size() - 1);
                    }
                }
                connection->GetPacket()->data.folder_id = task.folder_id;
                folders_[task.folder_id].queue.Push(connection);
                folders_[task.folder_id].exist = true;
                BOOST_LOG_TRIVIAL(info) << "[SM]: Sending task to create new folder";
                std::thread folder_run(WorkInThread, &folders_[task.folder_id].queue, w_settings_,
                                       connection->GetPacket()->login);
                folders_[task.folder_id].thread = std::move(folder_run);
                break;
            }
            case common::enums::operators::CONNECT: {
                short idx = connection->GetPacket()->data.folder_id;
                folders_[idx].exist = true;
                std::thread folder_run(WorkInThread, &folders_[task.folder_id].queue, w_settings_,
                                       connection->GetPacket()->login);
                folders_[task.folder_id].thread = std::move(folder_run);
                folders_[idx].queue.Push(connection);
                break;
            }
            case common::enums::operators::KILL:
            case common::enums::operators::DISCONNECT: {
                short idx = connection->GetPacket()->data.folder_id;
                folders_[idx].queue.Push(connection);
                folders_[idx].exist = false;
//                folders_[task.folder_id].thread.detach();
                // TODO(rowbotman): check how is it work
                //                folders_.erase(folders_.begin() + idx);
                break;
            }

            default: {
                short idx = connection->GetPacket()->data.folder_id;
                if (folders_[idx].exist) {
                    folders_[idx].queue.Push(connection);
                } else {
                    packet.ret_value = common::enums::response_type::NOT_FOUND;
                    connection->SendData(packet);
                }
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

void ServerManager::SetSettings(utils::Settings& settings) {
    w_settings_ = utils::WorkerSettings(settings.data_path, settings.do_backup);
}

ServerManager::ServerManager(common::utils::Queue<std::shared_ptr<network::Connection>>& task_queue)
    : task_queue_(task_queue), folders_(8), is_run_(false) {
    users_ = std::make_unique<auth::Authorization>();
}
short ServerManager::FindEmpty_() {
    auto it = std::find_if(folders_.begin(), folders_.end(),
                           [](const VirtualFolder& folder) { return !folder.exist; });
    if (it != folders_.end()) {
        return it - folders_.begin();
    }
    return -1;
}

}  // namespace engine
}  // namespace db
}  // namespace failless