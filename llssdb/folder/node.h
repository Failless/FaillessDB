// Node - контейнер для TM, DW и прочего
#ifndef LLSSDB_FOLDER_NODE_H_
#define LLSSDB_FOLDER_NODE_H_

#include <iostream>
#include <string>
#include "data_worker.h"
#include "log_manager.h"
#include "task_worker.h"

class Node {
 public:
    explicit Node(std::string directory);
    Node() = default;
    ~Node() = default;

    virtual bool TestCall() = 0;

    TaskWorker task_worker_;

 private:
    DataWorker data_worker_;
    LogManager log_manager_;
};

class TestNode : public Node {
 public:
    TestNode() = default;
    bool TestCall() override {
        std::cout << "test call" << std::endl;
        return true;
    }
};

#endif  // LLSSDB_FOLDER_NODE_H_
