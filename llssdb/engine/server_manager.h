#ifndef FAILLESS_LLSSDB_ENGINE_SERVER_MANAGER_H_
#define FAILLESS_LLSSDB_ENGINE_SERVER_MANAGER_H_

#include <map>
#include <queue>
#include <string>
#include <utility>
#include "llss3p/utils/queue.h"
#include "llssdb/auth/authorization.h"
#include "llssdb/engine/manager_interface.h"
#include "llssdb/folder/task_worker.h"
#include "llssdb/network/transfer/adapter.h"

namespace failless {
namespace db {
namespace engine {

class ServerManager : public IServerManager {
 public:
    ServerManager() = delete;
    explicit ServerManager(common::utils::Queue<std::shared_ptr<network::Connection>>& task_queue);
    ~ServerManager() override = default;

    void SetTask(utils::Task task) override;
    void Reload() override;
    void Run() override;
    void Stop() override;
    void SetSettings(utils::Settings& settings) override;

 protected:
    bool Execute_(utils::Task& task) override;

 private:
    int CreateFolder_(boost::uuids::uuid& client_id);
    bool KillFolder_(int folder_id);
    bool RedirectTask_(utils::Task& task);
    common::enums::operators HandleRequest_(utils::Task& Task);

    common::utils::Queue<std::shared_ptr<network::Connection>>& task_queue_;
    std::vector<std::unique_ptr<folder::ITaskWorker>> folders_;
    bool is_run_ = false;
    std::unique_ptr<auth::IAuthorization> users_;
};

}  // namespace engine
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_ENGINE_SERVER_MANAGER_H_