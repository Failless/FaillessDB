/*
TaskWorker (TW)
    1. Принимает таски от SM добавляет в очередь.
    2. Раздаёт из очереди своему DW
*/

#ifndef LLSSDB_FOLDER_TASK_WORKER_H
#define LLSSDB_FOLDER_TASK_WORKER_H


#include "task.h"
#include "data_worker.h"

#include <queue>
#include <string>

class ITaskWorker {
public:
    explicit ITaskWorker() : task_queue_(nullptr), data_queue_(nullptr) {};
    virtual ~ITaskWorker() = default;

    int AddTask(const Task& task) {
        DoTask(task);
        return EXIT_SUCCESS;
    };
private:
    virtual bool IsEmpty() = 0;
    int DoTask(const Task& task) {
        switch (task.command) {
            // TODO: write them in most frequent order
            case Task::NEW:
                break;
            case Task::KILL:
                break;
            case Task::CREATE:
                Create();
                break;
            case Task::READ:
                Read();
                break;
            case Task::UPDATE:
                Update();
                break;
            case Task::DELETE:
                Delete();
                break;
            default:
                return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }
    virtual void Create() = 0;
    virtual void Read() = 0;
    virtual void Update() = 0;
    virtual void Delete() = 0;

    std::queue<Task>* task_queue_;
    DataWorker data_worker_;
    std::queue<uint8_t>* data_queue_;
};


class TaskWorker : public ITaskWorker {
public:
    ~TaskWorker() override = default;

private:
    bool IsEmpty() override {
        return true;
    }

//    void Create()
};


#endif // LLSSDB_FOLDER_TASK_WORKER_H
