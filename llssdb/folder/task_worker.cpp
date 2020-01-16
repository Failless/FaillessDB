#include "llssdb/folder/task_worker.h"

#include <memory>
#include <thread>
#include <unistd.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <utility>

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
      : input_queue_(queue),
        user_path_(std::move(storage_path)),
        max_memory_( (sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGE_SIZE) - 1048576 ) / 2 ),
        do_backup_(_do_backup),
        alive_(true) {
    boost::filesystem::create_directory("/tmp/failless");
    if ( boost::filesystem::create_directory(user_path_) ) {
        BOOST_LOG_TRIVIAL(debug) << "[TW]: Created new folder at " << user_path_;
    }

    /// Find amount of users' databases
    for (size_t folder_id = 1; folder_id < UINT_MAX; ++folder_id) {
        if ( boost::filesystem::exists(user_path_ + "/" + std::to_string(folder_id)) ) {
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
        case common::enums::operators::KILL:  // Destroy DB
            BOOST_LOG_TRIVIAL(debug) << "[TW]: Received command KILL";
            //            SendAnswer_(conn, Destroy_(conn->GetPacket()->data), false);
            SendAnswer_(conn, enums::response_type::OK, false);
            break;
        case common::enums::operators::CREATE:  // create new folder for the same user
            BOOST_LOG_TRIVIAL(debug) << "[TW]: Received command CREATE";
            SendAnswer_(conn, Create_(), false);
            break;
        case common::enums::operators::DISCONNECT:  // Kill thread
            BOOST_LOG_TRIVIAL(debug) << "[TW]: Received command DISCONNECT";
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
    auto it = cache_.find(data.key);
    InMemoryData& existed = it->second;
    if ( it != cache_.end() ) {
        fs_->Remove(data.key);
        existed.value.clear();
        existed.value.shrink_to_fit();
        existed.in_memory = false;
        cur_memory_ -= existed.size;    // don't forget to change tmp.size !
    }

    /// Create_ key-value pair(s) on hdd
    enums::response_type result = fs_->Set(data);

    /// Update_ in-memory storage
    if (result == enums::response_type::OK) {
        PrepareCache_(data.size);
        if (it == cache_.end()) {
            cache_.emplace(std::make_pair(
                    data.key,
                    InMemoryData {
                        std::move(data.value),
                        data.size,
                        true
                    }));
        } else {
            existed = InMemoryData{std::move(data.value), data.size, true};
        }
        queue_.emplace(boost::posix_time::microsec_clock::local_time(), data.key);
        cur_memory_ += data.size;
    }

    BOOST_LOG_TRIVIAL(debug) << "[TW]: Value of size " << data.size << " was loaded into RAM";
    return result;
}

enums::response_type TaskWorker::Read_(common::utils::Data& data) {
    auto it = cache_.find(data.key);

    /// Check for existence of this key
    if ( it == cache_.end() ) {
        return enums::response_type::NOT_FOUND;
    }
    /// Get data from cache if it's in there
    else if ( it->second.in_memory ) {
        data.size = it->second.size;
        data.value = it->second.value;
        UpdateCache_(data.key);
        BOOST_LOG_TRIVIAL(debug) << "[TW]: \"" << data.key << "\" retrieved from RAM";
        return enums::response_type::OK;
    }
    /// Get data from HDD and put it into cache
    else {
        auto response = fs_->Get(data.key, data.value, data.size);
        PrepareCache_(data.size);
        it->second.value = data.value;
        it->second.size = data.size;
        it->second.in_memory = true;
        BOOST_LOG_TRIVIAL(debug) << "[TW]: \"" << data.key << "\" loaded from HDD into RAM";
        return response;
    }
}

enums::response_type TaskWorker::Delete_(common::utils::Data& data) {
    auto it = cache_.find(data.key);

    /// Check for existence of this key
    if ( it == cache_.end() ) {
        return enums::response_type::NOT_FOUND;
    }

    /// Delete_ key-value pair on HDD
    enums::response_type response = fs_->Remove(data.key);

    /// Update_ in-memory storage
    cur_memory_ -= it->second.size;
    cache_.erase(data.key.c_str());  // no, it's not redundant
    queue_.erase(std::find_if(  // yeah it's O(n) sorry
        queue_.begin(),
        queue_.end(),
        [key = data.key](const auto& pair){ return pair.second == key; }));
    BOOST_LOG_TRIVIAL(debug) << "[TW]: \"" << data.key << "\" erased from RAM";
    return response;
}

void TaskWorker::LoadCache_() {
    fs_->LoadCache(cache_, queue_, max_memory_, cur_memory_);
}

void TaskWorker::ClearCache_() {
    /// Clear vector and free space it occupied
    for (auto& it : cache_) {
        it.second.value.clear();
        it.second.value.shrink_to_fit();
        it.second.in_memory = false;
        cur_memory_ -= it.second.size;
    }
    BOOST_LOG_TRIVIAL(debug) << "[TW]: Unloaded everything from RAM";
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

    LoadCache_();
    return common::enums::response_type::OK;
}

void TaskWorker::PrepareCache_(long bytes) {
    while ( cur_memory_ + bytes > max_memory_ ) {
        InMemoryData& to_clear = cache_.find(queue_.begin()->second)->second;
        to_clear.value.clear();
        to_clear.value.shrink_to_fit();
        to_clear.in_memory = false;

        cur_memory_ -= to_clear.size;
        BOOST_LOG_TRIVIAL(debug) << "[TW]: \"" << queue_.begin()->second << "\" removed from RAM";
        queue_.erase(queue_.begin());
    }
}

void TaskWorker::UpdateCache_(const std::string& key) {
    queue_.erase(std::find_if(  // yeah it's O(n) sorry
            queue_.begin(),
            queue_.end(),
            [key](const auto& pair){ return pair.second == key; }));
    queue_.emplace(boost::posix_time::microsec_clock::local_time(), key);
    BOOST_LOG_TRIVIAL(debug) << "[TW]: \"" << key << "\" updated in cache";
}

}  // namespace folder
}  // namespace db
}  // namespace failless

// TODO(EgorBedov): make cache more dynamic
// TODO(EgorBedov): replace queue_ with container that sorts by key and finds by value
//  or store timestamp (delete and updatecache)
