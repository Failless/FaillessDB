/*
TaskWorker (TW)
    1. Принимает таски от SM добавляет в очередь.
    2. Раздаёт из очереди своему DW
*/
#ifndef LLSSDB_FOLDER_TASK_WORKER_H_
#define LLSSDB_FOLDER_TASK_WORKER_H_

#include <queue>
#include <string>
#include "llssdb/folder/data_worker.h"
#include "llssdb/folder/task.h"

class ITaskWorker {
 public:
    explicit ITaskWorker(DataWorker* _data_worker) : data_worker_(_data_worker){};
    //    TaskWorker() = default;
    virtual ~ITaskWorker() = default;

    int AddTask(const Task& task) {
        IsEmpty();
        CompleteTask();
        return EXIT_SUCCESS;
    };

 private:
    virtual bool IsEmpty() = 0;
    virtual int CompleteTask() = 0;

    std::queue<Task> task_queue_;
    DataWorker* data_worker_;
};

class TaskWorker : public ITaskWorker {
    ~TaskWorker() override = default;
    bool IsEmpty() override { return true; }
    int CompleteTask() override { return EXIT_SUCCESS; }
};

#endif  // LLSSDB_FOLDER_TASK_WORKER_H_
