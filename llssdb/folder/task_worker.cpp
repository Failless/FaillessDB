#include <boost/filesystem.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>

#include "llssdb/folder/task_worker.h"
#include "llssdb/folder/in_memory_data.h"
#include "llssdb/utils/task.h"

#define MAX_BUFFER 20000

namespace failless {
namespace db {
namespace folder {

TaskWorker::TaskWorker(common::utils::Queue<std::shared_ptr<network::Connection>>& queue,
                   const std::string& db_path)
          : input_queue_(queue),
            fs_(std::make_unique<FileSystem>(db_path)),
            path_(db_path),
            alive_(true)
            {
                LoadInMemory();
            }

void TaskWorker::Work() {
    /// Check input_queue_ for emptiness
    while ( alive_ ) {
        if ( !input_queue_.IsEmpty() ) {
            DoTask(input_queue_.Pop());
        } else {
            sleep(1);
        }
    }
}

int TaskWorker::DoTask(std::shared_ptr<network::Connection> conn) {
    switch (conn->GetPacket()->command) {
        case common::enums::operators::GET:
            Read(conn->GetPacket()->data);
            puts("GET worked");
            break;
        case common::enums::operators::SET:
            Set(conn->GetPacket()->data);
            puts("SET worked");
            break;
        case common::enums::operators::UPDATE:
            Update(conn->GetPacket()->data);
            puts("UPDATE worked");
            break;
        case common::enums::operators::DELETE:
            Delete(conn->GetPacket()->data);
            puts("DELETE worked");
            break;
        case common::enums::operators::CREATE:  // create new folder for the same user
            Create(conn->GetPacket()->data);
            puts("CREATE worked");
            break;
        case common::enums::operators::KILL:    // finish work
            puts("KILL worked");
            alive_ = false;
            break;
        default:
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

bool TaskWorker::Set(common::utils::Data& data) {
    /// Create key-value pair(s) on hdd
    bool result = fs_->Set(
            data.key,
            const_cast<uint8_t *>(data.value.data()),
            data.size);

    // TODO(EgorBedov): Send answer to socket

    /// Update in-memory storage
    if (result) {
        // TODO(EgorBedov): check RAM condition before loading in-memory
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

    size_t size = 0;

    std::cout << "Found it ";
    /// Grab file from memory if it's in there
    if ( !local_storage_.empty() && (local_storage_.at(data.key).in_memory) ) {
        data_out.size = local_storage_.at(data.key).size;
        data_out.value = local_storage_.at(data.key).value;
        size = true;
        std::cout << "in-memory\n" << std::endl;
    } else {
        auto value_out = new uint8_t[MAX_BUFFER];
        size = fs_->Get(data.key, value_out);
        data_out.value = std::vector(value_out[0], value_out[size - 1]);
        data_out.value.shrink_to_fit();
        std::cout << "On disk\n" << std::endl;
        delete[] value_out;
        // the data is not present in local storage for a reason
        // so first check RAM condition and then load
    }

    // TODO(EgorBedov): Send data_out to socket

    return size;
}

bool TaskWorker::Update(common::utils::Data& data) {
    bool result = true;
    // TODO(EgorBedov): Modify key-value pair(s) on hdd
    //   https://github.com/facebook/rocksdb/wiki/Merge-Operator

    // TODO(EgorBedov): Send answer to socket

    // TODO(EgorBedov): Update in-memory storage

    return result;
}

bool TaskWorker::Delete(common::utils::Data& data) {
    /// Delete key-value pair(s) on hdd
    bool result = fs_->Remove(data.key);

    // TODO(EgorBedov): Send answer to socket

    /// Update in-memory storage
    if (local_storage_.at(data.key).in_memory) {
        local_storage_.erase(data.key.c_str());
    }
    return result;
}

void TaskWorker::LoadInMemory() {
    fs_->LoadInMemory(local_storage_);
}

void TaskWorker::UnloadFromMemory() {
    /// Clear vector and free space it occupied
    for (auto &it : local_storage_) {
        it.second.value.clear();
        it.second.value.shrink_to_fit();
        it.second.in_memory = false;
    }
}

bool TaskWorker::Create(common::utils::Data &data) {
    /// Replacing old folder_id in path
    std::string new_path = path_;
    while ( new_path.back() != '/' ) {
        new_path.pop_back();
    }
    new_path += std::to_string(data.folder_id);

    if ( boost::filesystem::exists(new_path) ) {
        // TODO(EgorBedov): write to socket that folder exists.
        //   is it my area of responsibility tho?
        return false;
    }

    /// Creating new folder
    path_ = new_path;
    fs_.reset();
    fs_ = std::make_unique<FileSystem>(path_);
    // could've just called CloseDB() and OpenDB(new_path) but they're private and not in interface
    // also constructor is pretty light-weight

    return false;
}

}  // namespace folder
}  // namespace db
}  // namespace failless
