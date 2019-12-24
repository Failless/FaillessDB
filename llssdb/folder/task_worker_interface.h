#ifndef FAILLESS_LLSSDB_FOLDER_TASK_WORKER_INTERFACE_H_
#define FAILLESS_LLSSDB_FOLDER_TASK_WORKER_INTERFACE_H_

#include <boost/noncopyable.hpp>

#include "llss3p/enums/operators.h"
#include "llssdb/folder/file_system_interface.h"
#include "llssdb/network/transfer/hookup.h"

namespace failless::db::folder {

class ITaskWorker : boost::noncopyable {
public:
    virtual ~ITaskWorker() = default;
    virtual void Work() = 0;
    virtual int DoTask(std::shared_ptr<network::Connection> conn) = 0;

protected:
    virtual common::enums::response_type Set_(common::utils::Data& data) = 0;
    virtual common::enums::response_type Read_(common::utils::Data& data) = 0;
    virtual common::enums::response_type Delete_(common::utils::Data& data) = 0;
    virtual common::enums::response_type Create_() = 0;
    virtual common::enums::response_type Connect_(common::utils::Data& data) = 0;
    //    virtual common::enums::response_type DestroyDB_() = 0;
};

}  // namespace failless::db::folder

#endif  // FAILLESS_LLSSDB_FOLDER_TASK_WORKER_INTERFACE_H_
