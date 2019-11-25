#ifndef LLSSDB_FOLDER_TASK_WORKER_H_
#define LLSSDB_FOLDER_TASK_WORKER_H_

#include <boost/noncopyable.hpp>
#include <queue>
#include <string>
#include "llssdb/common/task.h"
#include "llssdb/folder/file_system.h"

namespace failless {
namespace db {
namespace folder {

using std::string;

class ITaskWorker : boost::noncopyable {
 public:
    explicit ITaskWorker(FileSystem *_fs) : task_queue_(nullptr), data_queue_(nullptr), fs(_fs) {}
    virtual ~ITaskWorker() = default;

    virtual int AddTask(const common::Task &task) = 0;

 protected:
    virtual bool IsEmpty() = 0;
    virtual int DoTask(const common::Task &task) = 0;
    virtual bool Create(const int8_t *value) = 0;
    virtual bool Read(const int8_t *value) = 0;
    virtual bool Update(const int8_t *value) = 0;
    virtual bool Delete(const int8_t *value) = 0;

    std::queue<common::Task> *task_queue_;
    std::queue<int8_t> *data_queue_;
    FileSystem *fs;
};

class TaskWorker : public ITaskWorker {
 public:
    explicit TaskWorker(FileSystem *_fs) : ITaskWorker(_fs){};
    ~TaskWorker() override = default;

    int AddTask(const common::Task &task) override {
        // Add async queue
        DoTask(task);
        return EXIT_SUCCESS;
    };

 protected:
    int DoTask(const common::Task &task) override {
        switch (task.command) {
            // TODO: order them in most frequent
            case common::operators::KILL:
                break;
            case common::operators::CREATE:
                Create(task.data);
                break;
            case common::operators::GET:
                Read(task.data);
                break;
            case common::operators::UPDATE:
                Update(task.data);
                break;
            case common::operators::DELETE:
                Delete(task.data);
                break;
            default:
                return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    bool IsEmpty() override { return true; }

    bool Create(const int8_t *value) override {
        string key = "0";
        // key is getting from value
        return fs->Set(key, value);
    }

    bool Read(const int8_t *value) override {
        string key = "0";
        fs->Get(key);
        return true;
    }

    bool Update(const int8_t *value) override {
        string key = "0";
        return fs->Set(key, value);
    }

    bool Delete(const int8_t *value) override {
        string key = "0";
        return fs->Remove(key);
    }
};

}  // namespace folder
}  // namespace db
}  // namespace failless

#endif  // LLSSDB_FOLDER_TASK_WORKER_H_
