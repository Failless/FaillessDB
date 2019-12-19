#ifndef FAILLESS_LLSSDB_FOLDER_TASK_WORKER_INTERFACE_H_
#define FAILLESS_LLSSDB_FOLDER_TASK_WORKER_INTERFACE_H_


#include <boost/noncopyable.hpp>
#include <string>

#include "llss3p/enums/operators.h"
#include "llss3p/utils/queue.h"
#include "llssdb/folder/in_memory_data.h"
#include "llssdb/network/transfer/hookup.h"
#include "llssdb/utils/task.h"

namespace failless::db::folder {

class ITaskWorker : boost::noncopyable {
public:
    virtual ~ITaskWorker() = default;
    virtual void Work() = 0;

protected:
    virtual int DoTask(std::shared_ptr<network::Connection> conn) = 0;
    virtual common::enums::response_type Set(common::utils::Data& data) = 0;
    virtual common::enums::response_type Read(common::utils::Data& data) = 0;
    virtual common::enums::response_type Update(common::utils::Data& data) = 0;
    virtual common::enums::response_type Delete(common::utils::Data& data) = 0;
    virtual common::enums::response_type Create() = 0;
};

}  // namespace failless


#endif  // FAILLESS_LLSSDB_FOLDER_TASK_WORKER_INTERFACE_H_
