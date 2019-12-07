#ifndef FAILLESS_LLSSDB_UTILS_SIGNAL_HANDLER_H
#define FAILLESS_LLSSDB_UTILS_SIGNAL_HANDLER_H

#include "llssdb/engine/manager_interface.h"
#include "llssdb/network/tcp_server_interface.h"

namespace failless {
namespace db {
namespace utils {

class SignalHandler {
 public:
    static void Handler(int signal);
    static void SetSignalHandlers();
    static void SignalSetter(std::shared_ptr<failless::db::engine::IServerManager> s_m,
                  std::shared_ptr<failless::db::network::ITcpServer> tcp_s);
    SignalHandler() = default;
    ~SignalHandler() = default;

 private:
    static void SafeTerminate_();
    static std::shared_ptr<failless::db::engine::IServerManager> server_manager_;
    static std::shared_ptr<failless::db::network::ITcpServer> tcp_server_;
};

}  // namespace utils
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_UTILS_SIGNAL_HANDLER_H
