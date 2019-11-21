#include <cstdlib>

#include "llssdb/common/task.h"
#include "llssdb/folder/task_worker.h"

namespace failless::db::folder {

// TODO(EgorBedov): this func is endless so make it void
int TaskWorker::AddTask(const common::Task &task) {
    /// Check task_queue for emptiness

    /// If not empty then do task
    DoTask(task);
    return EXIT_SUCCESS;
};


int TaskWorker::DoTask(const common::Task &task) {
    switch (task.command) {
        // TODO: order them in most frequent
        case common::operators::KILL:
            break;
        case common::operators::CREATE:
            Create(task);
            break;
        case common::operators::GET:
            Read(task);
            break;
        case common::operators::UPDATE:
            Update(task);
            break;
        case common::operators::DELETE:
            Delete(task);
            break;
        default:
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


bool TaskWorker::Create(const common::Task &task) {
    /// Parse key(s) from task.query - TODO(EgorBedov): add multiple key handling
    std::string key = *task.query;

    /// Create key-value pair(s) on hdd
    bool result = fs_.Set(key, *task.data);

    /// Send answer to data_queue_

    /// Update in-memory storage

    return result;
}


bool TaskWorker::Read(const common::Task &task) {
    /// Parse key(s) from task.query - TODO(EgorBedov): add multiple key handling
    std::string key = *task.query;

    /// Read the key(s)
    bool result = fs_.Get(key);

    /// Send answer to data_queue_
    return result;
}


bool TaskWorker::Update(const common::Task &task) {
    /// Parse key(s) from task.query - TODO(EgorBedov): add multiple key handling
    std::string key = *task.query;

    /// Modify key-value pair(s) on hdd
    bool result = fs_.Set(key, *task.data);

    /// Send answer to data_queue_

    /// Update in-memory storage

    return result;
}


bool TaskWorker::Delete(const common::Task &task) {
    /// Parse key(s) from task.query - TODO(EgorBedov): add multiple key handling
    std::string key = *task.query;

    /// Delete key-value pair(s) on hdd
    bool result = fs_.Remove(key);

    /// Send answer

    /// Update in-memory storage

    return result;
}

} // namespace failless::db::folder
