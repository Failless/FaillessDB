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
    int8_t value;   // Набор байт

    Task(Command _command, int8_t _value) : command(_command), value(_value) {};
};

#endif // LLSSDB_FOLDER_TASK_H
