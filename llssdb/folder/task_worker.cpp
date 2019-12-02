#include <cstdlib>
#include <iostream>
#include <string>

#include "llss3p/enums/operators.h"
#include "llssdb/folder/task_worker.h"
#include "llssdb/utils/task.h"

namespace failless {
namespace db {
namespace folder {

TaskWorker::TaskWorker(const std::string &db_path) : ITaskWorker(db_path) {
    /// Retrieve all k-v pairs from hdd in local_storage_
    //    fs_.LoadInMemory(local_storage_);
}

// TODO(EgorBedov): this func is endless so make it void
int TaskWorker::AddTask(const utils::Task &task) {
    /// Check input_queue_ for emptiness

    /// If not empty then do task
    DoTask(task);
    return EXIT_SUCCESS;
};

int TaskWorker::DoTask(const utils::Task &task) {
    switch (task.command) {
        // TODO: order them in most frequent
        case common::enums::operators::KILL:
            break;
        case common::enums::operators::SET:
            Set(task);
            break;
        case common::enums::operators::GET:
            Read(task);
            break;
        case common::enums::operators::UPDATE:
            Update(task);
            break;
        case common::enums::operators::DELETE:
            Delete(task);
            break;
        default:
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

bool TaskWorker::Set(const utils::Task &task_in) {
    /// Create key-value pair(s) on hdd
    bool result = fs_.Set(*task_in.payload.key, task_in.payload.value, task_in.payload.size);

    /// Send answer to output_queue_
    /// Update in-memory storage

    if (result) {
        std::cout << "{" << *task_in.payload.key << ": " << task_in.payload.size << "} was set\n";
    }
    return result;
}

bool TaskWorker::Read(const utils::Task &task_in) {
    utils::Task task_out;
    task_out.client_id = task_in.client_id;
    task_out.payload.key = new std::string(*task_in.payload.key);
    task_out.command = task_in.command;

    bool result;

    if (local_storage_ && (local_storage_->at(*task_in.payload.key).in_memory)) {
        task_out.payload.value = local_storage_->at(*task_in.payload.key).value;
        result = true;
        std::cout << "In-Memory ";
    } else {
        result = fs_.Get(*task_in.payload.key, task_out.payload.value);
        std::cout << "On HDD ";
    }

    std::cout << task_out.payload.value << std::endl;

    /// Send answer to output_queue_
    //    output_queue_->push(task_out);
    return result;
}

bool TaskWorker::Update(const utils::Task &task_in) {
    bool result = true;
    // TODO(EgorBedov): https://github.com/facebook/rocksdb/wiki/Merge-Operator
    /// Modify key-value pair(s) on hdd

    /// Send answer to output_queue_

    /// Update in-memory storage

    return result;
}

bool TaskWorker::Delete(const utils::Task &task_in) {
    /// Delete key-value pair(s) on hdd
    bool result = fs_.Remove(*task_in.payload.key);

    /// Send answer

    /// Update in-memory storage
    if (local_storage_) local_storage_->erase(*task_in.payload.key);

    return result;
}

}  // namespace folder
}  // namespace db
}  // namespace failless
