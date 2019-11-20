#include "llssdb/folder/node.h"
/*
#include <utility>

Node::Node(std::string directory) :
    task_worker_(&data_worker_),
    data_worker_(DataWorker(std::move(directory))) {}
*/
namespace failless {
namespace db {
namespace folder {

bool Node::TestCall() { return true; }

}  // namespace folder
}  // namespace db
}  // namespace failless
