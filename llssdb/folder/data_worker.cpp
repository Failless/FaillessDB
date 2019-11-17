#include "llssdb/folder/data_worker.h"

#include <iostream>
#include <utility>

DataWorker::DataWorker(std::string directory) : directory_(std::move(directory)) {}

int DataWorker::Create() { return 0; }

int DataWorker::Read() { return 0; }

int DataWorker::Modify() { return 0; }

int DataWorker::Remove() { return 0; }

// DataWorker::DataWorker(string directory) : directory_(directory) {}
