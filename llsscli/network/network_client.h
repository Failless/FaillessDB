#ifndef LLSSCLI_NETWORK_CLIENT_H_
#define LLSSCLI_NETWORK_CLIENT_H_

#include <string>
#include "llss3p/utils/queue.h"
#include "llsscli/config/config.h"

#include "llsscli/network/network_client_interface.h"

namespace failless {
namespace client {
namespace network {

class NetworkClient : public NetworkClientInterface {
 public:
    NetworkClient() = default;
    explicit NetworkClient(std::shared_ptr<config::NetworkConfig>& config);
    ~NetworkClient() override = default;

    size_t AddUserTask(std::shared_ptr<std::stringstream>& current_task,
                       std::shared_ptr<std::function<size_t()>>& callback) override;
    size_t OpenConnection() override;
    size_t Close() override;

 private:
    void OnConnect_(const boost::system::error_code& error_code,
                    tcp::resolver::iterator end_point_iter,
                    std::shared_ptr<config::NetworkConnectTask>& task) override;
    void OnReceive_(const boost::system::error_code& ErrorCode,
                    std::shared_ptr<tcp::socket>& socket) override;
    void OnSend_(const boost::system::error_code& error_code, std::shared_ptr<tcp::socket>& socket,
                 std::shared_ptr<std::string>& str_task) override;
    void DoClose_(std::shared_ptr<tcp::socket>& socket) override;

    std::vector<unsigned char> content_buffer_vector_;

    config::NetworkConfig config_;
    std::shared_ptr<common::utils::Queue<std::shared_ptr<config::NetworkConnectTask>>>
        user_socket_queue_;

    static const size_t buflen_ = 200;
    char recieve_buffer_[buflen_ * 2]{};
};

}  // namespace network
}  // namespace client
}  // namespace failless

#endif  // LLSSCLI_NETWORK_CLIENT_H_
