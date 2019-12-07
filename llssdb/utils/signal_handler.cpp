#include "llssdb/utils/signal_handler.h"
#include <csignal>

#include <utility>

namespace failless {
namespace db {
namespace utils {

std::shared_ptr<failless::db::engine::IServerManager> server_manager_;
std::shared_ptr<failless::db::network::ITcpServer> tcp_server_;

void SignalHandler::SafeTerminate_() {
    server_manager_->Stop();
    // tcp_server_->Stop();
}

void SignalHandler::SignalSetter(std::shared_ptr<failless::db::engine::IServerManager> s_m,
                             std::shared_ptr<failless::db::network::ITcpServer> tcp_s) {
    server_manager_ = std::move(s_m);
    tcp_server_ = std::move(tcp_s);
    SetSignalHandlers();
}

void SignalHandler::Handler(int signal) {
    switch (signal) {
        case SIGTERM:
        case SIGINT:
            SafeTerminate_();
            break;
        default:
            break;
    }
}

void SignalHandler::SetSignalHandlers() {
     signal(SIGTERM, Handler);
     signal(SIGINT, Handler);
}

}  // namespace utils
}  // namespace db
}  // namespace failless
