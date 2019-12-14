#include <boost/filesystem.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>

#include "llssdb/folder/task_worker.h"
#include "llssdb/folder/in_memory_data.h"
#include "llssdb/utils/task.h"

#define MAX_BUFFER 20000

namespace failless::db::folder {

using common::enums::response_type;

void TaskWorker::SendAnswer(std::shared_ptr<network::Connection>& conn, response_type result, bool read = false) {
    // Prepare return_packet
    conn->GetPacket()->ret_value = static_cast<int>(result);
    if ( !read ) {
        conn->GetPacket()->data.value = {};
    }
    conn->SendData(*(conn->GetPacket()));
}

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
            SendAnswer(conn, Read(conn->GetPacket()->data), true);
            puts("GET worked");
            break;
        case common::enums::operators::SET:
            SendAnswer(conn, Set(conn->GetPacket()->data), false);
            puts("SET worked");
            break;
        case common::enums::operators::UPDATE:
            SendAnswer(conn, Update(conn->GetPacket()->data), false);
            puts("UPDATE worked");
            break;
        case common::enums::operators::DELETE:
            SendAnswer(conn, Delete(conn->GetPacket()->data), false);
            puts("DELETE worked");
            break;
        case common::enums::operators::CREATE:  // create new folder for the same user
            SendAnswer(conn, Create(conn->GetPacket()->data), false);
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

response_type TaskWorker::Set(common::utils::Data& data) {
    /// Create key-value pair(s) on hdd
    response_type result = fs_->Set(
            data.key,
            const_cast<uint8_t *>(data.value.data()),
            data.size);

    /// Update in-memory storage
    if ( result == response_type::OK ) {
        // TODO(EgorBedov): check RAM condition before loading in-memory
        local_storage_.emplace(std::make_pair(data.key, InMemoryData(data.value, data.size, true)));
        std::cout << "{" << data.key << ": "
                  << local_storage_.at(data.key).size
                  << "(size)} was set both in HDD and in memory\n"
                  << std::endl;
    }
    return result;
}

response_type TaskWorker::Read(common::utils::Data& data) {
    auto response = response_type::SERVER_ERROR;

    common::utils::Data data_out;
    data_out.key = data.key;

    std::cout << "Found it ";
    /// Grab file from memory if it's in there
    if ( !local_storage_.empty() && (local_storage_.at(data.key).in_memory) ) {
        data_out.size = local_storage_.at(data.key).size;
        data_out.value = local_storage_.at(data.key).value;
        std::cout << "in-memory\n" << std::endl;
        response = response_type::OK;
    } else {
        auto value_out = new uint8_t[MAX_BUFFER];
        size_t size_out = 0;
        response = fs_->Get(data.key, value_out, size_out);
        data_out.value = std::vector(value_out[0], value_out[size_out - 1]);
        data_out.value.shrink_to_fit();
        std::cout << "On disk\n" << std::endl;
        delete[] value_out;
        // the data is not present in local storage for a reason
        // so first check RAM condition and then load
    }
    return response;
}

response_type TaskWorker::Update(common::utils::Data& data) {
    bool result = true;
    // TODO(EgorBedov): Modify key-value pair(s) on hdd
    //   https://github.com/facebook/rocksdb/wiki/Merge-Operator

    // TODO(EgorBedov): Update in-memory storage

    return response_type::OK;
}

response_type TaskWorker::Delete(common::utils::Data& data) {
    /// Delete key-value pair(s) on hdd
    response_type response = fs_->Remove(data.key);

    /// Update in-memory storage
    if (local_storage_.at(data.key).in_memory) {
        local_storage_.erase(data.key.c_str()); // no, it's not redundant
    }
    return response;
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

response_type TaskWorker::Create(common::utils::Data &data) {
    /// Replacing old folder_id in path
    std::string new_path = path_;
    while ( new_path.back() != '/' ) {
        new_path.pop_back();
    }
    new_path += std::to_string(data.folder_id);

    if ( boost::filesystem::exists(new_path) ) {
        // TODO(EgorBedov): write to socket that folder exists.
        //   is it my area of responsibility tho?
        return response_type::EXIST;
    }

    /// Creating new folder
    path_ = new_path;
    fs_.reset();
    fs_ = std::make_unique<FileSystem>(path_);
    // could've just called CloseDB() and OpenDB(new_path) but they're private and not in interface
    // also constructor is pretty light-weight

    return response_type::OK;
}

}  // namespace failless::db::folder
