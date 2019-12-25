#ifndef FAILLESS_LLSSDB_UTILS_TASK_H_
#define FAILLESS_LLSSDB_UTILS_TASK_H_


#include <memory>
#include <string>
#include <utility>

#include <boost/chrono/system_clocks.hpp>
#include <boost/uuid/uuid.hpp>

#include "llss3p/enums/operators.h"
#include "llss3p/utils/data.h"
#include "llss3p/utils/packet.h"

namespace failless {
namespace db {
namespace utils {

struct ServerTask {
    common::enums::operators command;
    std::string* login;
    std::string* password;
    short folder_id = 0;
    ServerTask() : command(common::enums::operators::GET), login(nullptr), password(nullptr) {}
    explicit ServerTask(common::utils::Packet& packet)
        : command(static_cast<common::enums::operators>(packet.command)),
          login(&packet.login),
          password(&packet.pass) {}
};

}  // namespace utils
}  // namespace db
}  // namespace failless

#endif // FAILLESS_LLSSDB_UTILS_TASK_H_
