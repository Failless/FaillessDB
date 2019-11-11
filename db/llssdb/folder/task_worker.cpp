/*
#include "llssdb/folder/task_worker.h"

#include <cstdlib>

int TaskWorker::Add_Task(const Task& task) {
    task_queue_.push(task);
    return EXIT_SUCCESS;
}

bool TaskWorker::IsEmpty() {
    return task_queue_.empty();
}

TaskWorker::TaskWorker(DataWorker *_data_worker) : data_worker_(_data_worker) {

}

int TaskWorker::CompleteTask() {
    Task current_task = task_queue_.front();
    switch ( current_task.command ) {
        case Task::CREATE:
            data_worker_->Create();
            break;
        case Task::READ:
            data_worker_->Read();
            break;
        case Task::UPDATE:
            data_worker_->Modify();
            break;
        case Task::DELETE:
            data_worker_->Remove();
            break;
        case Task::NEW:
        case Task::KILL:
        default:
            return EXIT_FAILURE;
    }

    task_queue_.pop();
    return EXIT_SUCCESS;
}
*/
