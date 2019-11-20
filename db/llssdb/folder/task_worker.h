#ifndef LLSSDB_FOLDER_TASK_WORKER_H
#define LLSSDB_FOLDER_TASK_WORKER_H


#include "task.h"
#include "file_system.h"

#include <boost/noncopyable.hpp>
#include <queue>
#include <string>

namespace failless::db::folder {

using std::string;

class ITaskWorker : boost::noncopyable {
public:
    explicit ITaskWorker(FileSystem *_fs) :
            task_queue_(nullptr),
            data_queue_(nullptr),
            fs(_fs) {}
    virtual ~ITaskWorker() = default;

    virtual int AddTask(const Task &task) = 0;

protected:
    virtual bool IsEmpty() = 0;
    virtual int DoTask(const Task &task) = 0;
    virtual bool Create(const int8_t *value) = 0;
    virtual bool Read(const int8_t &value) = 0;
    virtual bool Update(const int8_t *value) = 0;
    virtual bool Delete(const int8_t &value) = 0;

    std::queue<Task> *task_queue_;
    std::queue<int8_t> *data_queue_;
    FileSystem *fs;
};

class TaskWorker : public ITaskWorker {
public:
    explicit TaskWorker(FileSystem *_fs) : ITaskWorker(_fs) {};
    ~TaskWorker() override = default;

    int AddTask(const Task &task) override {
        // Add async queue
        DoTask(task);
        return EXIT_SUCCESS;
    };

protected:
    int DoTask(const Task &task) override {
        switch (task.command) {
            // TODO: order them in most frequent
            case Task::NEW:
                break;
            case Task::KILL:
                break;
            case Task::CREATE:
                Create(&task.value);
                break;
            case Task::READ:
                Read(task.value);
                break;
            case Task::UPDATE:
                Update(&task.value);
                break;
            case Task::DELETE:
                Delete(task.value);
                break;
            default:
                return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    bool IsEmpty() override {
        return true;
    }

    bool Create(const int8_t *value) override {
        string key = "0";
        // key is getting from value
        return fs->Set(key, value);
    }

    bool Read(const int8_t &value) override {
        string key = "0";
        fs->Get(key);
        return true;
    }

    bool Update(const int8_t *value) override {
        string key = "0";
        return fs->Set(key, value);
    }

    bool Delete(const int8_t &value) override {
        string key = "0";
        return fs->Remove(key);
    }
};

}

#endif // LLSSDB_FOLDER_TASK_WORKER_H
