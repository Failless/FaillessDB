/*
TaskWorker (TW)
    1. Принимает таски от SM добавляет в очередь.
    2. Раздаёт из очереди своему DW
*/

#ifndef LLSSDB_FOLDER_TASK_WORKER_H
#define LLSSDB_FOLDER_TASK_WORKER_H


#include "task.h"
#include "file_system.h"

#include <boost/noncopyable.hpp>
#include <queue>
#include <string>

class ITaskWorker : boost::noncopyable {
public:
    explicit ITaskWorker(FileSystem* _fs) :
      task_queue_(nullptr),
      data_queue_(nullptr),
      fs(_fs) {}
    virtual ~ITaskWorker() = default;

    virtual int AddTask(const Task& task) = 0;

protected:
    virtual bool IsEmpty() = 0;
    virtual int DoTask(const Task& task) = 0;
    virtual bool Create(const u_int8_t& data) = 0;
    virtual bool Read(const u_int8_t& data) = 0;
    virtual bool Update(const u_int8_t& data) = 0;
    virtual bool Delete(const u_int8_t& data) = 0;

    std::queue<Task>* task_queue_;
    std::queue<u_int8_t>* data_queue_;
    FileSystem* fs;
};

class TaskWorker : public ITaskWorker {
public:
    explicit TaskWorker(FileSystem* _fs) : ITaskWorker(_fs) {};
    ~TaskWorker() override = default;

    int AddTask(const Task& task) override {
      // Add async queue
      DoTask(task);
      return EXIT_SUCCESS;
    };

protected:
    int DoTask(const Task& task) override {
        switch (task.command) {
          // TODO: order them in most frequent
            case Task::NEW:
              break;
            case Task::KILL:
              break;
            case Task::CREATE:
              Create(task.data);
              break;
            case Task::READ:
              Read(task.data);
              break;
            case Task::UPDATE:
              Update(task.data);
              break;
            case Task::DELETE:
              Delete(task.data);
              break;
            default:
              return EXIT_FAILURE;
            }
        return EXIT_SUCCESS;
    }

    bool IsEmpty() override {
        return true;
    }

    bool Create(const u_int8_t& data) override {
        int key = 0;
        // key is getting from data
        return fs->Set(key, data);
    }

    bool Read(const u_int8_t& data) override {
        int key = 0;
        fs->Get(key);
        return true;
    }

    bool Update(const u_int8_t& data) override {
        int key = 0;
        return fs->Set(key, data);
    }

    bool Delete(const u_int8_t& data) override {
        int key = 0;
        return fs->Remove(key);
    }
};


#endif // LLSSDB_FOLDER_TASK_WORKER_H
