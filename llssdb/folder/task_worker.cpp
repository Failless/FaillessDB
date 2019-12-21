#include "llssdb/folder/task_worker.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <memory>
#include "llss3p/enums/operators.h"
#include "llssdb/folder/in_memory_data.h"
#include "llssdb/network/transfer/hookup.h"

namespace failless {
namespace db {
namespace folder {

namespace enums = common::enums;

void TaskWorker::SendAnswer_(std::shared_ptr<network::Connection>& conn,
                             enums::response_type result, bool read) {
    // Prepare return_packet
    conn->GetPacket()->ret_value = static_cast<int>(result);
    if (!read) {
        conn->GetPacket()->data.value = {};
    }
    conn->SendData(*(conn->GetPacket()));
}

TaskWorker::TaskWorker(common::utils::Queue<std::shared_ptr<network::Connection>>& queue,
                       std::string storage_path)
    : input_queue_(queue), user_path_(storage_path), alive_(true) {
    //    if ( storage_path.empty() ) {
    storage_path = "/tmp/failless";
    boost::filesystem::create_directory(storage_path);
    //    }
//    user_path_ = std::move(storage_path) + "/" + input_queue_.Pop()->GetPacket()->login;
    boost::filesystem::create_directory(user_path_);
    BOOST_LOG_TRIVIAL(debug) << "[TW]: Created new folder at " << user_path_;

    //    if (!boost::filesystem::exists(user_path_)) {
    //        boost::filesystem::create_directory(user_path_);
    //    } else {
    //        BOOST_LOG_TRIVIAL(debug) << "[TW]: Found folder at " << user_path_;
    //    }

    /// Find amount of users' databases TODO(EgorBedov): improve it later
    for (size_t folder_id = 1; folder_id < UINT_MAX; ++folder_id) {
        if (boost::filesystem::exists(user_path_ + "/" + std::to_string(folder_id))) {
            dbs_.push_back(folder_id);
        } else {
            break;
        }
    }
    if (dbs_.empty()) {
        BOOST_LOG_TRIVIAL(info) << "[TW]: Creating new db for new user at " << user_path_;
        // Don't invoke virtual member functions from constructor
        // TODO(EgorBedov): rewrite it
        Create_();
    }
}

void TaskWorker::Work() {
    /// Check input_queue_ for emptiness
    while (alive_) {
        if (!input_queue_.IsEmpty()) {
            DoTask(input_queue_.Pop());
        } else {
            sleep(1);
        }
    }
}

int TaskWorker::DoTask(std::shared_ptr<network::Connection> conn) {
    // TODO: remove it
    if (conn->GetPacket()->data.key.empty()) {
        std::vector<std::string> words;
        boost::split(words, conn->GetPacket()->request, boost::is_any_of(" "));
        conn->GetPacket()->data.key = words[1];
    }
    switch (conn->GetPacket()->command) {
        case common::enums::operators::GET:
            BOOST_LOG_TRIVIAL(debug) << "[TW]: Received command GET";
            SendAnswer_(conn, Read_(conn->GetPacket()->data), true);
            break;
        case common::enums::operators::SET:
            BOOST_LOG_TRIVIAL(debug) << "[TW]: Received command SET";
            SendAnswer_(conn, Set_(conn->GetPacket()->data), false);
            break;
        case common::enums::operators::UPDATE:
            BOOST_LOG_TRIVIAL(debug) << "[TW]: Received command UPDATE";
            SendAnswer_(conn, Update_(conn->GetPacket()->data), false);
            break;
        case common::enums::operators::DELETE:
            BOOST_LOG_TRIVIAL(debug) << "[TW]: Received command DELETE";
            SendAnswer_(conn, Delete_(conn->GetPacket()->data), false);
            break;
        case common::enums::operators::CONNECT:
            BOOST_LOG_TRIVIAL(debug) << "[TW]: Received command CONNECT";
            SendAnswer_(conn, Connect_(conn->GetPacket()->data), false);
            break;
        /// Destroy DB
        case common::enums::operators::KILL:
            BOOST_LOG_TRIVIAL(debug) << "[TW]: Received command KILL which destroys db (under construction)";
//            SendAnswer_(conn, Destroy_(conn->GetPacket()->data), false);
            SendAnswer_(conn, enums::response_type::OK, false);
            break;
        case common::enums::operators::CREATE:  // create new folder for the same user
            BOOST_LOG_TRIVIAL(debug) << "[TW]: Received command CREATE";
            SendAnswer_(conn, Create_(), false);
            break;
        /// Kill thread
        case common::enums::operators::DISCONNECT:
            BOOST_LOG_TRIVIAL(debug) << "[TW]: Received command DISCONNECT which for some reason kills thread";
            BOOST_LOG_TRIVIAL(info) << "[TW]: TaskWorker finished working";
            alive_ = false;
            break;
        default:
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

enums::response_type TaskWorker::Set_(common::utils::Data& data) {
    /// Create_ key-value pair(s) on hdd
    enums::response_type result = fs_->Set(data);

    /// Update_ in-memory storage
    if (result == enums::response_type::OK) {
        // TODO(EgorBedov): check RAM condition before loading in-memory
        auto valid = local_storage_.emplace(
            std::make_pair(data.key, InMemoryData(data.value, data.size, true)));
        if (valid.second) {
            BOOST_LOG_TRIVIAL(debug)
                << "[TW]: Value of size " << data.size << " was loaded into RAM";
        } else {
            BOOST_LOG_TRIVIAL(warning)
                << "[TW]: Value of size " << data.size << " was not loaded into RAM";
        }
    }
    return result;
}

enums::response_type TaskWorker::Read_(common::utils::Data& data) {
    auto response = enums::response_type::SERVER_ERROR;

    common::utils::Data data_out;
    data_out.key = data.key;

    /// Grab file from memory if it's in there
    if (!local_storage_.empty() && (local_storage_.at(data.key).in_memory)) {
        data_out.size = local_storage_.at(data.key).size;
        data_out.value = local_storage_.at(data.key).value;
        BOOST_LOG_TRIVIAL(debug) << "[TW]: \"" << data.key << "\" retrieved from RAM";
        response = enums::response_type::OK;
    } else {
        response = fs_->Get(data.key, data.value, data.size);
        // the data is not present in local storage for a reason
        // so first check RAM condition and then load
    }
    return response;
}

enums::response_type TaskWorker::Update_(common::utils::Data& data) {
    bool result = true;
    // TODO(EgorBedov): Modify key-value pair(s) on hdd
    //   https://github.com/facebook/rocksdb/wiki/Merge-Operator

    // TODO(EgorBedov): Update_ in-memory storage

    return enums::response_type::OK;
}

enums::response_type TaskWorker::Delete_(common::utils::Data& data) {
    /// Delete_ key-value pair(s) on hdd
    enums::response_type response = fs_->Remove(data.key);

    /// Update_ in-memory storage
    if (local_storage_.at(data.key).in_memory) {
        local_storage_.erase(data.key.c_str());  // no, it's not redundant
        BOOST_LOG_TRIVIAL(debug) << "[TW]: \"" << data.key << "\" erased from RAM";
    }
    return response;
}

void TaskWorker::LoadInMemory_() { fs_->LoadInMemory(local_storage_); }

void TaskWorker::UnloadFromMemory_() {
    /// Clear vector and free space it occupied
    for (auto& it : local_storage_) {
        it.second.value.clear();
        it.second.value.shrink_to_fit();
        it.second.in_memory = false;
        BOOST_LOG_TRIVIAL(debug) << "[TW]: Unloaded everythin from RAM";
    }
}

enums::response_type TaskWorker::Create_() {
    size_t new_id = 1;
    if (!dbs_.empty()) {
        new_id = dbs_.back() + 1;
    }
    dbs_.push_back(new_id);
    std::string new_folder_path = user_path_ + "/" + std::to_string(new_id);

    boost::filesystem::create_directory(new_folder_path);
    boost::filesystem::create_directory(new_folder_path + "/db");
    boost::filesystem::create_directory(new_folder_path + "/backup");

    BOOST_LOG_TRIVIAL(info) << "[TW]: Created new folder at " << new_folder_path;

    /// Switch FileSystem
    fs_ = std::make_unique<FileSystem>(new_folder_path);

    return enums::response_type::OK;
}

common::enums::response_type TaskWorker::Connect_(common::utils::Data& data) {
    std::string new_folder_path = user_path_ + "/" + std::to_string(data.folder_id);
    if (!boost::filesystem::exists(new_folder_path)) {
        BOOST_LOG_TRIVIAL(error) << "[TW]: Folder is not present at " << new_folder_path;
        return common::enums::NOT_FOUND;
    }

    /// Switch FileSystem
    fs_.reset();
    fs_ = std::make_unique<FileSystem>(new_folder_path);
    BOOST_LOG_TRIVIAL(info) << "[TW]: Connected to db at " << new_folder_path;

    return common::enums::response_type::OK;
}

}  // namespace folder
}  // namespace db
}  // namespace failless
