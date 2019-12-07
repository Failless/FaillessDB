#ifndef FAILLESS_LLSSDB_FOLDER_WORKER_INTERFACE_H_
#define FAILLESS_LLSSDB_FOLDER_WORKER_INTERFACE_H_

#include <boost/noncopyable.hpp>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include "llss3p/utils/queue.h"
#include "llssdb/folder/value_info.h"
#include "llssdb/network/transfer/hookup.h"
#include "llssdb/utils/task.h"

namespace failless {
namespace db {
namespace folder {

class ITaskWorker : boost::noncopyable {
 public:
    virtual ~ITaskWorker() = default;
    virtual void Work() = 0;

 protected:
    virtual int DoTask(std::shared_ptr<network::Connection> conn) = 0;
    virtual bool Set(common::utils::Data& data) = 0;
    virtual bool Read(common::utils::Data& data) = 0;
    virtual bool Update(common::utils::Data& data) = 0;
    virtual bool Delete(common::utils::Data& data) = 0;


};

}  // namespace folder
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_FOLDER_WORKER_INTERFACE_H_
