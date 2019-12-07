// TODO(EgorBedov): consider time when it's necessary to update in-memory storage
//   after every query, or later ?

#ifndef FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_
#define FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_

#include "llss3p/utils/queue.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/worker_interface.h"
#include "llssdb/network/transfer/hookup.h"

namespace failless {
namespace db {
namespace folder {

class TaskWorker : public ITaskWorker {
 public:
    TaskWorker(common::utils::Queue<std::shared_ptr<network::Connection>> &queue,
               std::string &db_path);
    ~TaskWorker() override = default;

    int AddTask(const utils::Task &task) override;
    void Work() override;

 protected:
    int DoTask(const utils::Task &task) override;

    /// These functions will handle the "cache"
    bool Set(const utils::Task &task_in) override;
    bool Read(const utils::Task &task_in) override;
    bool Update(const utils::Task &task_in) override;
    bool Delete(const utils::Task &task_in) override;
    void LoadInMemory();
    void UnloadFromMemory();
    common::utils::Queue<std::shared_ptr<network::Connection>> &input_queue_;
    std::map<std::string, ValueInfo> *local_storage_ = nullptr;
    common::utils::Queue<utils::Task> *output_queue_;
    std::unique_ptr<FileSystemInterface> fs_;
};

}  // namespace folder
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_
