#include <cstdlib>
#include <iostream>
#include <memory>

#include "llssdb/folder/task_worker.h"
#include "llssdb/folder/value_info.h"
#include "llssdb/utils/task.h"

namespace failless {
namespace db {
namespace folder {

// TODO(EgorBedov): this func is endless so make it void
int TaskWorker::AddTask(const utils::Task &task) {
    /// Check input_queue_ for emptiness

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
    bool result = fs_->Set(task_in.payload.key, const_cast<uint8_t *>(task_in.payload.value.data()),
                          task_in.payload.size);

    /// Send answer to output_queue_

    if (task_in.payload.value.empty()) {
        std::cout << "value is null!" << std::endl;
    }
    /// Update in-memory storage
    if (result) {
        // TODO(EgorBedov): check for memory condition first
        //  if it's odd - emplace empty
        local_storage_->emplace(task_in.payload.key,
                                ValueInfo(const_cast<uint8_t *>(task_in.payload.value.data()),
                                          task_in.payload.size, true));
        std::cout << "{" << task_in.payload.key << ": "
                  << (*local_storage_)[task_in.payload.key].size
                  << "(size)} was set both in HDD and in memory\n"
                  << std::endl;
    }
    return result;
}

bool TaskWorker::Read(const utils::Task &task_in) {
    utils::Task task_out;
    task_out.client_id = task_in.client_id;
    task_out.payload.key = task_in.payload.key;
    task_out.command = task_in.command;

    size_t result = 0;

    std::cout << "Inside of map" << std::endl;
    std::map<std::string, ValueInfo>::iterator it;
    for (it = local_storage_->begin(); it != local_storage_->end(); ++it) {
        std::cout << "{" << it->first << ": " << it->second.size << "(size)} and it's "
                  << (it->second.value == nullptr ? "" : "not ") << "a nullptr" << std::endl;
    }

    std::cout << "Found it ";
    /// Grab file from memory if it's in there
//    if (local_storage_->at(task_in.payload.key).in_memory) {
//        task_out.payload.value = local_storage_->at(task_in.payload.key).value;
    // TODO(EgorBedov): rewrite it on smth good
    uint8_t value[20000];
    if (local_storage_ && (local_storage_->at(task_in.payload.key).in_memory)) {
        task_out.payload.value = std::vector(&local_storage_->at(task_in.payload.key).value[0],
                                             &local_storage_->at(task_in.payload.key).value[20000]);
        //        task_out.payload.value = local_storage_->at(task_in.payload.key).value;
        result = true;
        std::cout << "in-memory" << std::endl;
    } else {
        result = fs_->Get(task_in.payload.key, value);
        task_out.payload.value = std::vector(&value[0], &value[20000]);
        std::cout << "On HDD ";
    }

    std::cout << task_out.payload.value.size() << std::endl;
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
    bool result = fs_->Remove(task_in.payload.key);

    /// Send answer

    /// Update in-memory storage
    if (local_storage_->at(task_in.payload.key).in_memory) {
        local_storage_->erase(task_in.payload.key);
    }
    return result;
}
void TaskWorker::Work() {
    // TODO(EgorBedov): implement your event loop here
}
TaskWorker::TaskWorker(common::utils::Queue<std::shared_ptr<network::Connection>> &queue,
                       std::string &db_path)
    : input_queue_(queue), fs_(std::make_unique<FileSystem>(db_path)) {}

void TaskWorker::LoadInMemory() { fs_->LoadInMemory(*local_storage_); }

void TaskWorker::UnloadFromMemory() {
    for (auto &it : *local_storage_) {
        delete [] it.second.value;
        it.second.in_memory = false;
    }
}

}  // namespace folder
}  // namespace db
}  // namespace failless
