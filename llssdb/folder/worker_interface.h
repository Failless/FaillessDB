#ifndef FAILLESS_LLSSDB_FOLDER_WORKER_INTERFACE_H_
#define FAILLESS_LLSSDB_FOLDER_WORKER_INTERFACE_H_

#include <boost/noncopyable.hpp>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include "llss3p/utils/queue.h"
#include "llssdb/folder/value_info.h"
#include "llssdb/utils/task.h"

namespace failless {
namespace db {
namespace folder {

class ITaskWorker : boost::noncopyable {
 public:
    virtual ~ITaskWorker() = default;
    // TODO(EgorBedov): command should be parsed from task.query
    virtual int AddTask(const utils::Task &task) = 0;
    virtual void Work() = 0;

 protected:
    virtual int DoTask(const utils::Task &task) = 0;
    virtual bool Set(const utils::Task &task_in) = 0;
    virtual bool Read(const utils::Task &task_in) = 0;
    virtual bool Update(const utils::Task &task_in) = 0;
    virtual bool Delete(const utils::Task &task_in) = 0;


};

}  // namespace folder
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_FOLDER_WORKER_INTERFACE_H_
