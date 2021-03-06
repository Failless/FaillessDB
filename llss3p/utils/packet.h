#ifndef FAILLESS_LLSS3P_UTILS_PACKET_H_
#define FAILLESS_LLSS3P_UTILS_PACKET_H_

#include <msgpack.hpp>
#include <string>
#include "llss3p/enums/operators.h"
#include "llss3p/utils/data.h"

namespace failless {
namespace common {
namespace utils {

struct Packet {
    std::string login{};
    std::string pass{};
    std::string request{};
    int command{};
    int ret_value{};
    Data data{};

    MSGPACK_DEFINE_MAP(login, pass, request, command, ret_value, data);

    Packet& operator=(const Packet& packet) = default;//{
//        login = packet.login;
//        pass = packet.pass;
//        request = packet.request;
//        command = packet.command;
//        ret_value = packet.ret_value;
//        data = packet.data;
//    };
    Packet() = default;
};

}  // namespace utils
}  // namespace common
}  // namespace failless

#endif  // FAILLESS_LLSS3P_UTILS_PACKET_H_
