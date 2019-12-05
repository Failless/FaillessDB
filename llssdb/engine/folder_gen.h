#ifndef FAILLESS_LLSSDB_ENGINE_FOLDER_GEN_H_
#define FAILLESS_LLSSDB_ENGINE_FOLDER_GEN_H_

#include "llss3p/utils/queue.h"
#include "llssdb/utils/settings.h"
#include "llssdb/utils/task.h"

namespace failless {
namespace db {
namespace engine {

void WorkInThread(const common::utils::Queue<utils::Task>& queue, utils::WorkerSettings& settings);

}  // namespace engine
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_ENGINE_FOLDER_GEN_H_
