#ifndef LLSSDB_ENGINE_SERVER_MANAGER_H_
#define LLSSDB_ENGINE_SERVER_MANAGER_H_

#include <map>
#include <queue>
#include <string>
#include "llssdb/folder/node.h"
#include "llssdb/folder/task.h"

namespace failless {
namespace db {
namespace engine {

using std::string;

class ServerManager {
 public:
    static ServerManager *Instance() {
        if (!sm_) {
            sm_ = new ServerManager;
            return sm_;
        }
        return sm_;
    }
    ServerManager(ServerManager const &) = delete;
    ServerManager &operator=(ServerManager const &) = delete;
    ~ServerManager() = default;

    int GetRequest(const string &request);
    int SendResponse(const string &response);

 private:
    ServerManager() { Instance(); }

    int HandleRequest();
    Task ParseRequest(const string &request);
    void CreateNode();
    void KillNode();
    void SendTask(const Task &task);

    static ServerManager *sm_;
    std::queue<string> request_queue_;
    std::map<int, Node *> active_nodes_;
};
}  // namespace engine
}  // namespace db
}  // namespace failless

#endif  // LLSSDB_ENGINE_SERVER_MANAGER_H_
