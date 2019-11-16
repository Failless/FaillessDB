#ifndef LLSSDB_FOLDER_TASK_H
#define LLSSDB_FOLDER_TASK_H

#include <string>
#include <utility>

using std::string;

struct Task {
    enum Command {
        CREATE,
        READ,
        UPDATE,
        DELETE,
        NEW,
        KILL
    };

    Command command;
    uint8_t data;   // Набор байт

    Task(Command _command, uint8_t _data) : command(_command), data(_data) {};
};

#endif // LLSSDB_FOLDER_TASK_H
