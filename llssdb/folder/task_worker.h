#ifndef FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_
#define FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_

#include <string>

#include "llss3p/utils/queue.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/file_system_interface.h"
#include "llssdb/folder/task_worker_interface.h"
#include "llssdb/network/transfer/hookup.h"

namespace failless {
namespace db {
namespace folder {

class TaskWorker : public ITaskWorker {
public:
    TaskWorker(common::utils::Queue<std::shared_ptr<network::Connection>> &queue,
               const std::string &db_path);
    ~TaskWorker() override = default;

    void Work() override;
    int DoTask(std::shared_ptr<network::Connection> conn) override;

protected:
    bool Set(common::utils::Data& data) override;
    bool Read(common::utils::Data& data) override;
    bool Update(common::utils::Data& data) override;
    bool Delete(common::utils::Data& data) override;
    bool Create(common::utils::Data& data) override;
    void LoadInMemory();
    void UnloadFromMemory();

    common::utils::Queue<std::shared_ptr<network::Connection>> &input_queue_;
    std::map<std::string, InMemoryData> local_storage_;
    std::unique_ptr<FileSystemInterface> fs_;
    std::string path_{};
    bool alive_ = false;
};

}  // namespace folder
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_
