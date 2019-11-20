#include "llssdb/folder/data_worker.h"
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
  std::shared_ptr<IDataWorker> data_worker(new DataWorker());
  data_worker->Create();
  return 0;
}