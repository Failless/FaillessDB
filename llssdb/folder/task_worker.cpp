#include "llssdb/folder/task_worker.h"

#include <memory>
#include <sys/types.h>

#ifdef __APPLE__
    #include <sys/sysctl.h>
#else
    #include <sys/sysinfo.h>
#endif

#include <thread>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

#include "llss3p/enums/operators.h"
#include "llssdb/folder/in_memory_data.h"
#include "llssdb/network/transfer/hookup.h"

namespace failless {
namespace db {
namespace folder {

namespace enums = common::enums;

void TaskWorker::SendAnswer_(std::shared_ptr<network::Connection>& conn,
                             enums::response_type result, bool read) {
    conn->GetPacket()->ret_value = static_cast<int>(result);
    if (!read) {
        conn->GetPacket()->data.value = {};
    }
    conn->SendData(*(conn->GetPacket()));
}

TaskWorker::TaskWorker(common::utils::Queue<std::shared_ptr<network::Connection>>& queue,
                       std::string storage_path,
                       bool _do_backup = false)
    : input_queue_(queue), user_path_(storage_path), do_backup_(_do_backup), alive_(true) {
    boost::filesystem::create_directory("/tmp/failless");
    if ( boost::filesystem::create_directory(user_path_) ) {
        BOOST_LOG_TRIVIAL(debug) << "[TW]: Created new folder at " << user_path_;
    }

    /// Find amount of users' databases
    for (size_t folder_id = 1; folder_id < UINT_MAX; ++folder_id) {
        if (boost::filesystem::exists(user_path_ + "/" + std::to_string(folder_id))) {
            dbs_.push_back(folder_id);
        } else {
            break;
        }
    }
    if (dbs_.empty()) {
        BOOST_LOG_TRIVIAL(info) << "[TW]: Creating new db for new user at " << user_path_;
        Create_();
    }
}

void TaskWorker::Work() {
    /// Check input_queue_ for emptiness
    while (alive_) {
        if (!input_queue_.IsEmpty()) {
            DoTask(input_queue_.Pop());
        } else {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    }
}

int TaskWorker::DoTask(std::shared_ptr<network::Connection> conn) {
    // TODO: remove it
    if (conn->GetPacket()->data.key.empty()) {
        std::vector<std::string> words;
        boost::split(words, conn->GetPacket()->request, boost::is_any_of(" "));
        if (words.size() > 1) {
            conn->GetPacket()->data.key = words[1];
        }
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
            BOOST_LOG_TRIVIAL(debug)
                << "[TW]: Received command KILL which destroys db (under construction)";
            //            SendAnswer_(conn, Destroy_(conn->GetPacket()->data), false);
            SendAnswer_(conn, enums::response_type::OK, false);
            break;
        case common::enums::operators::CREATE:  // create new folder for the same user
            BOOST_LOG_TRIVIAL(debug) << "[TW]: Received command CREATE";
            SendAnswer_(conn, Create_(), false);
            break;
        /// Kill thread
        case common::enums::operators::DISCONNECT:
            BOOST_LOG_TRIVIAL(debug)
                << "[TW]: Received command DISCONNECT which for some reason kills thread";
            BOOST_LOG_TRIVIAL(info) << "[TW]: TaskWorker finished working";
            SendAnswer_(conn, enums::response_type::OK, false);
            alive_ = false;
            break;
        default:
            SendAnswer_(conn, enums::response_type::NOT_DONE, false);
            break;
    }
    return EXIT_SUCCESS;
}

enums::response_type TaskWorker::Set_(common::utils::Data& data) {
    /// Delete existing key
    auto it = local_storage_.find(data.key);
    if ( it == local_storage_.end() ) {
        fs_->Remove(data.key);
    }

    /// Create_ key-value pair(s) on hdd
    enums::response_type result = fs_->Set(data);

    /// Update_ in-memory storage
    if (result == enums::response_type::OK) {
        // TODO(EgorBedov): check RAM condition before loading in-memory
        bool valid = false;
        if (it != local_storage_.end()) {
            it->second.value = data.value;
            it->second.size = data.size;
            valid = true;
        } else {
            valid =
                local_storage_
                    .emplace(std::make_pair(data.key, InMemoryData(data.value, data.size, true)))
                    .second;
        }

        if (valid) {
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

    /// Grab file from memory if it's in there
    if (!local_storage_.empty() && (local_storage_.at(data.key).in_memory)) {
        data.size = local_storage_.at(data.key).size;
        data.value = local_storage_.at(data.key).value;
        BOOST_LOG_TRIVIAL(debug) << "[TW]: \"" << data.key << "\" retrieved from RAM";
        response = enums::response_type::OK;
    } else {
        response = fs_->Get(data.key, data.value, data.size);
        // the data is not present in local storage for a reason
        // so first check RAM condition and then load
    }
    return response;
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

void TaskWorker::AdjustCache_() {
    int CPUs = sysconf(_SC_NPROCESSORS_ONLN);

    int mib [] = { CTL_HW, HW_MEMSIZE };
    int64_t value = 0;
    size_t length = sizeof(value);

    if ( -1 == sysctl(mib, 2, &value, &length, NULL, 0) ) {
        return;
    }
}

void TaskWorker::LoadInMemory_() { fs_->LoadInMemory(local_storage_); }

void TaskWorker::UnloadFromMemory_() {
    /// Clear vector and free space it occupied
    for (auto& it : local_storage_) {
        it.second.value.clear();
        it.second.value.shrink_to_fit();
        it.second.in_memory = false;
        BOOST_LOG_TRIVIAL(debug) << "[TW]: Unloaded everything from RAM";
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
    fs_ = std::make_unique<FileSystem>(new_folder_path, do_backup_);

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
    fs_ = std::make_unique<FileSystem>(new_folder_path, do_backup_);
    BOOST_LOG_TRIVIAL(info) << "[TW]: Connected to db at " << new_folder_path;

    return common::enums::response_type::OK;
}

}  // namespace folder
}  // namespace db
}  // namespace failless
