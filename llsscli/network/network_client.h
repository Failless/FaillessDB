#ifndef LLSSCLI_NETWORK_CLIENT_H_
#define LLSSCLI_NETWORK_CLIENT_H_

#include <string>
#include "llsscli/config/config.h"

#include "llsscli/network/network_client_interface.h"

namespace failless {
namespace client {
namespace network {

class NetworkClient : public NetworkClientInterface {
 public:
    NetworkClient() = default;
    NetworkClient(boost::asio::io_service& io_service, config::NetworkConfig config);
    ~NetworkClient() = default;

    void Close() override;

 private:
    void OnConnect_(const boost::system::error_code& error_code,
                    tcp::resolver::iterator end_point_iter) override;
    void OnReceive_(const boost::system::error_code& error_code) override;
    void OnSend_(const boost::system::error_code& error_code) override;
    void DoClose_() override;

    boost::asio::io_service& io_service_;
    tcp::socket socket_;

    std::string send_buffer_;
    static const size_t buflen_ = 100;
    char recieve_buffer_[buflen_ * 2];

    config::NetworkConfig config_;
};

}  // namespace network
}  // namespace client
}  // namespace failless

#endif  // LLSSCLI_NETWORK_CLIENT_H_
