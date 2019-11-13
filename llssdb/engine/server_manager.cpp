
#include "server_manager.h"

#include <iostream>
#include <queue>
#include <string>
#include <vector>

using std::queue;
using std::string;
using std::vector;
namespace failless {
namespace db {
namespace engine {
ServerManager* ServerManager::sm_;

}
} // namespace llssdb
} // namespace failless

// ServerManager::~ServerManager() {
//    for ( auto & active_node : active_nodes_ ) {
//        delete(active_node.second);
//    }
//}
//
// int ServerManager::GetRequest(const std::string& request) {
//    request_queue_.push(request);
//    // TODO: just for the sake of tests_cli
//    Task task(Task::CREATE, request);
//    SendTask(task);
//    // should delete that later cuz i wanna sleep so bad
//    return EXIT_SUCCESS;
//}
//
// int ServerManager::SendResponse(const std::string& response) {
//    return EXIT_SUCCESS;
//}
//
// int ServerManager::HandleRequest() {
//    string current_request = request_queue_.front();
//    Task current_task = ParseRequest(current_request);
//    switch ( current_task.command ) {
//        case Task::NEW:     CreateNode();   break;
//        case Task::KILL:    KillNode();     break;
//        case Task::CREATE:
//        case Task::READ:
//        case Task::UPDATE:
//        case Task::DELETE:  SendTask(current_task);     break;
//        default:            return EXIT_FAILURE;
//    }
//    request_queue_.pop();
//    return EXIT_SUCCESS;
//}
//
// Task ServerManager::ParseRequest(const string& request) {}
//
// void ServerManager::SendTask(const Task& task) {
////    int pos = 0;
////    active_nodes_[pos]->task_worker_.Add_Task(task);
//}
