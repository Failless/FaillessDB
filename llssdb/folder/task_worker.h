#ifndef FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_
#define FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_

#include <cstdlib>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "llss3p/enums/operators.h"
#include "llss3p/utils/queue.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/file_system_interface.h"
#include "llssdb/folder/in_memory_data.h"
#include "llssdb/folder/task_worker_interface.h"
#include "llssdb/network/transfer/hookup.h"

namespace failless {
namespace db {
namespace folder {

class TaskWorker : public ITaskWorker {
 public:
    TaskWorker(common::utils::Queue<std::shared_ptr<network::Connection>>& queue,
               const std::string& folder_path);
    ~TaskWorker() override = default;

    void Work() override;
    int DoTask(std::shared_ptr<network::Connection> conn) override;

 protected:
    common::enums::response_type Set_(common::utils::Data& data) override;
    common::enums::response_type Read_(common::utils::Data& data) override;
    common::enums::response_type Update_(common::utils::Data& data) override;
    common::enums::response_type Delete_(common::utils::Data& data) override;
    common::enums::response_type Create_() override;

    void LoadInMemory_();
    void UnloadFromMemory_();

    virtual void SendAnswer_(std::shared_ptr<network::Connection>& conn,
                             common::enums::response_type result, bool read);

    common::utils::Queue<std::shared_ptr<network::Connection>>& input_queue_;
    std::unordered_map<std::string, InMemoryData> local_storage_;
    std::unique_ptr<FileSystemInterface> fs_;
    std::string user_path_{};
    std::vector<size_t> dbs_{};
    bool alive_ = false;
    const int kMaxBuffer_ = 20000;
};

}  // namespace folder
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_
