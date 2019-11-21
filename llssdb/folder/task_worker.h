// TODO(EgorBedov): consider time when it's necessary to update in-memory storage
// after every query, or later ?

#ifndef LLSSDB_FOLDER_TASK_WORKER_H_
#define LLSSDB_FOLDER_TASK_WORKER_H_

#include <boost/noncopyable.hpp>
#include <queue>
#include <string>
#include <map>

#include "llssdb/common/task.h"
#include "llssdb/folder/file_system.h"

namespace failless::db::folder {

using std::string;

class ITaskWorker : boost::noncopyable {
public:
    explicit ITaskWorker(const std::string& db_path) :
        task_queue_(nullptr),
        data_queue_(nullptr),
        local_storage_(nullptr),
        fs_(db_path) {}
    virtual ~ITaskWorker() = default;

    virtual int AddTask(const common::Task &task) = 0;

protected:
    virtual int DoTask(const common::Task &task) = 0;
    virtual bool Create(const common::Task &task) = 0;
    virtual bool Read(const common::Task &task) = 0;
    virtual bool Update(const common::Task &task) = 0;
    virtual bool Delete(const common::Task &task) = 0;

    std::queue<common::Task> *task_queue_;
    std::queue<int8_t> *data_queue_;
    std::map<int, int8_t>* local_storage_;  // In-Memory data set
    FileSystem fs_;
};

class TaskWorker : public ITaskWorker {
public:
    explicit TaskWorker(const std::string& db_path) : ITaskWorker(db_path){};
    ~TaskWorker() override = default;

    int AddTask(const common::Task &task) override;

protected:
    int DoTask(const common::Task &task) override;

    /// These functions will handle the "cache"
    bool Create(const common::Task &task) override;
    bool Read(const common::Task &task) override;
    bool Update(const common::Task &task) override;
    bool Delete(const common::Task &task) override;
};

}  // namespace failless::db::folder

#endif  // LLSSDB_FOLDER_TASK_WORKER_H_
