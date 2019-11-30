#include <cstdlib>
#include <iostream>
#include <string>

#include "llssdb/common/task.h"
#include "llssdb/folder/value_info.h"
#include "llssdb/folder/task_worker.h"

namespace failless::db::folder {

TaskWorker::~TaskWorker() {
    UnloadFromMemory();
}

// TODO(EgorBedov): this func is endless so make it void
int TaskWorker::AddTask(const common::Task &task) {
    /// Check input_queue_ for emptiness

    DoTask(task);
    return EXIT_SUCCESS;
};


int TaskWorker::DoTask(const common::Task &task) {
    switch (task.command) {
        // TODO: order them in most frequent
        case common::operators::KILL:
            break;
        case common::operators::SET:
            Set(task);
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


bool TaskWorker::Set(const common::Task &task_in) {
    /// Create key-value pair(s) on hdd
    bool result = fs_.Set(*task_in.payload.key, task_in.payload.value, task_in.payload.size);

    /// Send answer to output_queue_

    if ( task_in.payload.value.get() == nullptr ) {
        std::cout << "value is null!" << std::endl;
    }
    /// Update in-memory storage
    if ( result ) {
        // TODO(EgorBedov): check for memory condition first
        //  if it's odd - emplace empty
        local_storage_.emplace(*task_in.payload.key,
            ValueInfo(task_in.payload.value, task_in.payload.size, true));
        std::cout << "{" << *task_in.payload.key << ": " <<
            local_storage_[*task_in.payload.key].size <<
            "(size)} was set both in HDD and in memory\n" << std::endl;
    }
    return result;
}


bool TaskWorker::Read(const common::Task &task_in) {
    common::Task task_out;
    task_out.client_id = task_in.client_id;
    task_out.payload.key = task_in.payload.key;
    task_out.command = task_in.command;

    bool result = false;

    std::cout << "Inside of map" << std::endl;
    std::map<std::string, ValueInfo>::iterator it;
    for ( it = local_storage_.begin(); it != local_storage_.end(); ++it ) {
        std::cout << "{" << it->first << ": " << it->second.size << "(size)} and it's " <<
            ( it->second.value == nullptr ? "" : "not " ) << "a nullptr" << std::endl;
    }

    std::cout << "Found it ";
    /// Grab file from memory if it's in there
    if ( local_storage_.at(*task_in.payload.key).in_memory ) {
        task_out.payload.value = local_storage_.at(*task_in.payload.key).value;
        result = true;
        std::cout << "in-memory" << std::endl;
    } else {
        result = fs_.Get(*task_in.payload.key, task_out.payload.value, task_out.payload.size);
        // TODO(EgorBedov): load in memory?
        std::cout << "on HDD" << std::endl;
    }


    /// Send answer to output_queue_
//    output_queue_->push(task_out);
    return result;
}


bool TaskWorker::Update(const common::Task &task_in) {
    bool result = true;
    // TODO(EgorBedov): https://github.com/facebook/rocksdb/wiki/Merge-Operator
    /// Modify key-value pair(s) on hdd

    /// Send answer to output_queue_

    /// Update in-memory storage

    return result;
}


bool TaskWorker::Delete(const common::Task &task_in) {
    /// Delete key-value pair(s) on hdd
    bool result = fs_.Remove(*task_in.payload.key);

    /// Send answer

    /// Update in-memory storage
    if ( local_storage_.at(*task_in.payload.key).in_memory )
        local_storage_.erase(*task_in.payload.key);

    return result;
}

void TaskWorker::LoadInMemory() {
     fs_.LoadInMemory_(local_storage_);
}

void TaskWorker::UnloadFromMemory() {
    for ( auto & it : local_storage_ ) {
        it.second.value.reset();
        it.second.in_memory = false;
    }
}


} // namespace failless::db::folder
