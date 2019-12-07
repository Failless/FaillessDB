#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>

#include "llssdb/folder/task_worker.h"
#include "llssdb/folder/value_info.h"
#include "llssdb/utils/task.h"

#define MAX_BUFFER 20000

namespace failless {
namespace db {
namespace folder {

TaskWorker::TaskWorker(common::utils::Queue<std::shared_ptr<network::Connection>>& queue,
                   const std::string& db_path)
          : input_queue_(queue),
            fs_(std::make_unique<FileSystem>(db_path))
            {
                LoadInMemory();
            }

void TaskWorker::Work() {
    /// Check input_queue_ for emptiness
    while ( true ) {
        if ( !input_queue_.IsEmpty() ) {
            DoTask(input_queue_.Pop());
        } else {
            sleep(1);
        }
    }
}

int TaskWorker::DoTask(std::shared_ptr<network::Connection> conn) {
    switch (conn->GetPacket()->command) {
        // TODO: order them in most frequent
        case common::enums::operators::KILL:
            break;
        case common::enums::operators::SET:
            Set(conn->GetPacket()->data);
            break;
        case common::enums::operators::GET:
            Read(conn->GetPacket()->data);
            break;
        case common::enums::operators::UPDATE:
            Update(conn->GetPacket()->data);
            break;
        case common::enums::operators::DELETE:
            Delete(conn->GetPacket()->data);
            break;
        default:
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

bool TaskWorker::Set(common::utils::Data& data) {
    /// Create key-value pair(s) on hdd
    bool result = fs_->Set(data.key, const_cast<uint8_t *>(data.value.data()),
                          data.size);

    /// Send answer to output_queue_

    /// Update in-memory storage
    // TODO(EgorBedov): check for memory condition first
    //  if it's odd - emplace empty
    if (result) {
        local_storage_.emplace(std::make_pair(data.key, InMemoryData(data.value, data.size, true)));
        std::cout << "{" << data.key << ": "
                  << local_storage_.at(data.key).size
                  << "(size)} was set both in HDD and in memory\n"
                  << std::endl;
    }
    return result;
}

bool TaskWorker::Read(common::utils::Data& data) {
    common::utils::Data data_out;
    data_out.key = data.key;

    size_t result = 0;

    std::cout << "Found it ";
    /// Grab file from memory if it's in there
    if ( !local_storage_.empty() && (local_storage_.at(data.key).in_memory) ) {
        data_out.size = local_storage_.at(data.key).size;
        data_out.value = local_storage_.at(data.key).value;
        result = true;
        std::cout << "in-memory\n" << std::endl;
    } else {
        auto value_out = new uint8_t[MAX_BUFFER];
        result = fs_->Get(data.key, value_out);
        data_out.value = std::vector(value_out[0], value_out[MAX_BUFFER]);
        std::cout << "On disk\n" << std::endl;
    }

    /// Send data_out to socket

    /// Consider loading data_out to local_storage_

    return result;
}

bool TaskWorker::Update(common::utils::Data& data) {
    bool result = true;
    // TODO(EgorBedov): https://github.com/facebook/rocksdb/wiki/Merge-Operator
    /// Modify key-value pair(s) on hdd

    /// Send answer to output_queue_

    /// Update in-memory storage

    return result;
}

bool TaskWorker::Delete(common::utils::Data& data) {
    /// Delete key-value pair(s) on hdd
    bool result = fs_->Remove(data.key);

    /// Send answer

    /// Update in-memory storage
    if (local_storage_.at(data.key).in_memory) {
        local_storage_.erase(data.key.c_str());
    }
    return result;
}

void TaskWorker::LoadInMemory() { fs_->LoadInMemory(local_storage_); }

void TaskWorker::UnloadFromMemory() {
    /// Clear vector and free space it occupied
    for (auto &it : local_storage_) {
        it.second.value.clear();
        it.second.value.shrink_to_fit();
        it.second.in_memory = false;
    }
}

}  // namespace folder
}  // namespace db
}  // namespace failless
