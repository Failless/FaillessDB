/*
TaskWorker (TW)
    1. Принимает таски от SM добавляет в очередь.
    2. Раздаёт из очереди своему DW
*/
#ifndef LLSSDB_FOLDER_TASK_WORKER_H_
#define LLSSDB_FOLDER_TASK_WORKER_H_

#include <queue>
#include <string>
#include "llssdb/common/task.h"
#include "llssdb/folder/data_worker.h"

namespace failless {
namespace db {
namespace folder {

class ITaskWorker {
 public:
    virtual ~ITaskWorker() = default;
    int AddTask(const common::Task& task) {
        IsEmpty_();
        CompleteTask_();
        return EXIT_SUCCESS;
    };

 protected:
    virtual bool IsEmpty_() = 0;
    virtual int CompleteTask_() = 0;

};

class TaskWorker : public ITaskWorker {
 public:
    TaskWorker();
    ~TaskWorker() override = default;
    bool IsEmpty_() override { return true; }
    int CompleteTask_() override { return EXIT_SUCCESS; }

 private:

    std::queue<common::Task> task_queue_;
    std::unique_ptr<IDataWorker> data_worker_;
};

}  // namespace folder
}  // namespace db
}  // namespace failless

#endif  // LLSSDB_FOLDER_TASK_WORKER_H_
