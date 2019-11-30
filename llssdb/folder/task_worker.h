// TODO(EgorBedov): consider time when it's necessary to update in-memory storage
// after every query, or later ?

#ifndef FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_
#define FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_

#include <boost/noncopyable.hpp>
#include <map>
#include <memory>
#include <queue>
#include <string>

#include "llss3p/utils/queue.h"
#include "llssdb/folder/file_system.h"
#include "llssdb/folder/value_info.h"
#include "llssdb/utils/task.h"

namespace failless::db::folder {

class ITaskWorker : boost::noncopyable {
 public:
    explicit ITaskWorker(const std::string &db_path)
        : local_storage_(nullptr), input_queue_(nullptr), output_queue_(nullptr), fs_(db_path) {}
    virtual ~ITaskWorker() = default;
    // TODO(EgorBedov): command should be parsed from task.query
    virtual int AddTask(const utils::Task &task) = 0;

 protected:
    virtual int DoTask(const utils::Task &task) = 0;
    virtual bool Set(const utils::Task &task_in) = 0;
    virtual bool Read(const utils::Task &task_in) = 0;
    virtual bool Update(const utils::Task &task_in) = 0;
    virtual bool Delete(const utils::Task &task_in) = 0;

    std::map<std::string, ValueInfo> *local_storage_;  // TODO(EgorBedov): string - type of key?
    common::utils::Queue<utils::Task> *input_queue_;
    common::utils::Queue<utils::Task> *output_queue_;
    FileSystem fs_;
};

class TaskWorker : public ITaskWorker {
 public:
    explicit TaskWorker(const std::string &db_path);
    ~TaskWorker() override = default;

    int AddTask(const utils::Task &task) override;

 protected:
    int DoTask(const utils::Task &task) override;

    /// These functions will handle the "cache"
    bool Set(const utils::Task &task_in) override;
    bool Read(const utils::Task &task_in) override;
    bool Update(const utils::Task &task_in) override;
    bool Delete(const utils::Task &task_in) override;
};

}  // namespace failless::db::folder

#endif  // FAILLESS_LLSSDB_FOLDER_TASK_WORKER_H_