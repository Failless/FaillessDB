#include "llssdb/folder/node.h"
/*
#include <utility>

Node::Node(std::string directory) :
    task_worker_(&data_worker_),
    data_worker_(DataWorker(std::move(directory))) {}
*/
#include "node.h"

bool Node::TestCall() { return true; }
