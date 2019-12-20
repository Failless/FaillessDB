#include "llssdb/folder/task_worker.h"

#include <climits>
#include <cstdlib>
#include <iostream>
#include <memory>

#include <boost/filesystem.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

#include "llss3p/enums/operators.h"
#include "llssdb/folder/in_memory_data.h"
#include "llssdb/network/transfer/hookup.h"

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
                       const std::string& user_path)
          : input_queue_(queue),
            user_path_(user_path),
            alive_(true)
            {
                /// Find amount of users' databases TODO(EgorBedov): improve it later
                for ( size_t folder_id = 0; folder_id < SIZE_T_MAX; ++folder_id ) {
                    if ( boost::filesystem::exists(user_path + "/" + std::to_string(folder_id)) ) {
                        dbs_.push_back(folder_id);
                    } else {
                        break;
                    }
                }
                if ( dbs_.empty() ) {
                    Create();
                    LoadInMemory();
                } else {
                    std::cout << "Welcome back!\nYou have next databases: ";
                    std::copy(
                            dbs_.begin(),
                            dbs_.end(),
                            std::ostream_iterator<size_t>(std::cout, " " ));
                    std::cout << "\nType OPEN <id> to open db" << std::endl;
                }
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
            break;
        case common::enums::operators::SET:
            SendAnswer(conn, Set(conn->GetPacket()->data), false);
            break;
        case common::enums::operators::UPDATE:
            SendAnswer(conn, Update(conn->GetPacket()->data), false);
            break;
        case common::enums::operators::DELETE:
            SendAnswer(conn, Delete(conn->GetPacket()->data), false);
            break;
        case common::enums::operators::CREATE:  // create new folder for the same user
            SendAnswer(conn, Create(), false);
            break;
        case common::enums::operators::KILL:
            BOOST_LOG_TRIVIAL(info) << "TaskWorker finished working";
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
        auto valid = local_storage_.emplace(std::make_pair(data.key, InMemoryData(data.value, data.size, true)));
        if ( valid.second ){
            BOOST_LOG_TRIVIAL(debug) << "Value of size " << data.size << " was loaded into RAM";
        } else {
            BOOST_LOG_TRIVIAL(warning) << "Value of size " << data.size << " was not loaded into RAM";
        }
    }
    return result;
}

response_type TaskWorker::Read(common::utils::Data& data) {
    auto response = response_type::SERVER_ERROR;

    common::utils::Data data_out;
    data_out.key = data.key;

    /// Grab file from memory if it's in there
    if ( !local_storage_.empty() && (local_storage_.at(data.key).in_memory) ) {
        data_out.size = local_storage_.at(data.key).size;
        data_out.value = local_storage_.at(data.key).value;
        BOOST_LOG_TRIVIAL(debug) << "\"" << data.key << "\" retrieved from RAM";
        response = response_type::OK;
    } else {
        auto value_out = new uint8_t[MAX_BUFFER];
        size_t size_out = 0;
        response = fs_->Get(data.key, value_out, size_out);
        data_out.value = std::vector(value_out[0], value_out[size_out - 1]);
        data_out.value.shrink_to_fit();
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
        BOOST_LOG_TRIVIAL(debug) << "\"" << data.key << "\" erased from RAM";
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
        BOOST_LOG_TRIVIAL(debug) << "Unloaded everythin from RAM";
    }
}

response_type TaskWorker::Create() {
    size_t new_id = 0;
    if ( !dbs_.empty() ) {
        new_id = dbs_.back() + 1;
    }
    dbs_.push_back(new_id);
    std::string new_folder_path = user_path_ + "/" + std::to_string(new_id);

    boost::filesystem::create_directory(new_folder_path);
    boost::filesystem::create_directory(new_folder_path + "/db");
    boost::filesystem::create_directory(new_folder_path + "/backup");

    /// Switch FileSystem
    fs_.reset();
    fs_ = std::make_unique<FileSystem>(new_folder_path);

    BOOST_LOG_TRIVIAL(info) << "Created new folder at " << new_folder_path;
    return response_type::OK;
}

}  // namespace failless::db::folder
