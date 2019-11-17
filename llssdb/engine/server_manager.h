#ifndef FAILLESS_LLSSDB_ENGINE_SERVER_MANAGER_H_
#define FAILLESS_LLSSDB_ENGINE_SERVER_MANAGER_H_

#include <map>
#include <queue>
#include <string>
#include "llssdb/engine/manager_interface.h"
#include "llssdb/folder/node.h"
#include "llssdb/folder/task.h"

namespace failless {
namespace db {
namespace engine {

class ServerManager : public IServerManager {
 public:
    ServerManager() = default;
    ~ServerManager() override = default;
    void SetTask(common::Task task) override;
    void Reload() override;
    void Run() override;
    void Stop() override;

 protected:
    bool Execute_(common::operators command) override;

 private:
    bool CreateFolder_();
    bool KillFolder_(int folder_id);
    bool RedirectTask_(common::Task& task);
    common::operators HandleRequest_(common::Task& Task);
};

}  // namespace engine
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_ENGINE_SERVER_MANAGER_H_
