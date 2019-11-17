#ifndef FAILLESS_LLSSDB_ENGINE_MANAGER_INTERFACE_H_
#define FAILLESS_LLSSDB_ENGINE_MANAGER_INTERFACE_H_

#include <boost/noncopyable.hpp>
#include "llssdb/common/operators.h"
#include "llssdb/common/settings.h"
#include "llssdb/common/task.h"

namespace failless {
namespace db {
namespace engine {

/**
 * This is the main class. It handle all requests and redirect these to concrete virtual folder.
 * Also this class make requests parsing and conversation with TCP server
 */
class IServerManager : boost::noncopyable {
 public:
    IServerManager() = default;
    virtual ~IServerManager() = 0;

    /**
     * Set task to common queue for inside task handler
     * @param task
     */
    virtual void SetTask(common::Task task) = 0;

    /**
     * Reload server. This method create server restart and reinitialize its settings.
     */
    virtual void Reload() = 0;

    /**
     * Main method of this class. It has event loop and it wait for any actions from server and
     * folders. Also in this method are created base preparing and parsing all queries
     */
    virtual void Run() = 0;

    /**
     * This method used for save server stopping by killing signal for example.
     */
    virtual void Stop() = 0;

    /**
     * It is the setter of this class. It set base settings of database
     * @param settings
     */
    virtual void SetSettings(common::Settings& settings) = 0;

 protected:
    /**
     * This method used for execute all queries or commands from client
     * @param command
     * @return
     */
    virtual bool Execute_(common::operators command) = 0;
};

}  // namespace engine
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_ENGINE_MANAGER_INTERFACE_H_
