#ifndef LLSSDB_FOLDER_TASK_H_
#define LLSSDB_FOLDER_TASK_H_

#include <string>
#include <utility>

using std::string;

struct Task {
    enum Command { CREATE, READ, UPDATE, DELETE, NEW, KILL };

    Command command;
    string data;

    Task(Command _command, string _data) : command(_command), data(std::move(_data)){};
};

#endif  // LLSSDB_FOLDER_TASK_H_