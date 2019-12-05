#include "llssdb/engine/folder_gen.h"
#include "llssdb/folder/task_worker.h"

namespace failless {
namespace db {
namespace engine {

void WorkInThread(common::utils::Queue<utils::Task>& queue, utils::WorkerSettings& settings) {
    std::unique_ptr<folder::ITaskWorker> worker(new folder::TaskWorker(queue, settings.db_path));
    worker->Work();
}

}  // namespace engine
}  // namespace db
}  // namespace failless
