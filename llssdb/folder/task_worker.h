#ifndef FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_
#define FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_


#include <string>

#include "llss3p/enums/operators.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/file_system_interface.h"
#include "llssdb/folder/task_worker_interface.h"
#include "llssdb/network/transfer/hookup.h"
#include "llss3p/utils/queue.h"

namespace failless::db::folder {

class TaskWorker : public ITaskWorker {
public:
    TaskWorker(common::utils::Queue<std::shared_ptr<network::Connection>> &queue,
               const std::string &db_path);
    ~TaskWorker() override = default;

    void Work() override;
    int DoTask(std::shared_ptr<network::Connection> conn) override;

protected:
    common::enums::response_type Set(common::utils::Data& data) override;
    common::enums::response_type Read(common::utils::Data& data) override;
    common::enums::response_type Update(common::utils::Data& data) override;
    common::enums::response_type Delete(common::utils::Data& data) override;
    common::enums::response_type Create(common::utils::Data& data) override;
    void LoadInMemory();
    void UnloadFromMemory();

    virtual void SendAnswer(std::shared_ptr<network::Connection>& conn,
                    common::enums::response_type result,
                    bool read);

    common::utils::Queue<std::shared_ptr<network::Connection>> &input_queue_;
    std::unordered_map<std::string, InMemoryData> local_storage_;
    std::unique_ptr<FileSystemInterface> fs_;
    std::string path_{};
    bool alive_ = false;
};

}  // namespace failless::db::folder

#endif  // FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_
