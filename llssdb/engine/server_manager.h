#ifndef FAILLESS_LLSSDB_ENGINE_SERVER_MANAGER_H_
#define FAILLESS_LLSSDB_ENGINE_SERVER_MANAGER_H_

#include <boost/lockfree/queue.hpp>
#include <map>
#include <queue>
#include <string>
#include <utility>
#include "llssdb/engine/manager_interface.h"
#include "llssdb/folder/task_worker.h"
#include "llssdb/network/connection.h"


namespace failless {
namespace db {
namespace engine {

class ServerManager : public IServerManager {
 public:
    ServerManager() = delete;
    explicit ServerManager(boost::lockfree::queue<network::ConnectionAdapter>& task_queue)
        : task_queue_(task_queue),
          //          folders_(0),  // I don't think that it's necessary but...
          is_run_(false){};
    ~ServerManager() override = default;

    void SetTask(common::Task task) override;
    void Reload() override;
    void Run() override;
    void Stop() override;
    void SetSettings(common::Settings& settings) override;

 protected:
    bool Execute_(common::Task& task) override;

 private:
    int CreateFolder_(boost::uuids::uuid& client_id);
    bool KillFolder_(int folder_id);
    bool RedirectTask_(common::Task& task);
    common::operators HandleRequest_(common::Task& Task);

    boost::lockfree::queue<network::ConnectionAdapter>& task_queue_;
    std::vector<std::unique_ptr<folder::ITaskWorker>> folders_;
    bool is_run_ = false;
};

}  // namespace engine
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_ENGINE_SERVER_MANAGER_H_
