/*
 * Singleton
 *
ServerManager (SM) принимает три вида запроса:
    1 - На создание ноды:
        Сам создаёт ноду
    2 - На CRUD:
        Посылает запрос дальше на ноду
    3 - На удаление ноды:
        Посылает запрос на ноду
        Удаляет из вектора рабочих нод

    Работают два потока:
     Один принимает и добавляет в очередь реквест
     Второй следит за наполнением очереди, парсит и раздает запросы
*/

#ifndef LLSSDB_ENGINE_SERVER_MANAGER_H_
#define LLSSDB_ENGINE_SERVER_MANAGER_H_

#include "llssdb/folder/task.h"
#include "llssdb/folder/node.h"
#include <map>
#include <queue>
#include <string>

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
} // namespace engine
} // namespace db
} // namespace failless

#endif // LLSSDB_ENGINE_SERVER_MANAGER_H_
