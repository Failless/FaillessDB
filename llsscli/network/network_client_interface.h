#ifndef LLSSCLI_NETWORK_CLIENT_INTERFACE_H_
#define LLSSCLI_NETWORK_CLIENT_INTERFACE_H_

// https://stackoverflow.com/questions/12990840/boost-async-tcp-client
// https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T

#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>

namespace failless {
namespace client {
namespace network {

using boost::asio::ip::tcp;

class NetworkClientInterface : boost::noncopyable {
 public:
    virtual ~NetworkClientInterface() = default;

    virtual size_t AddUserTask(std::shared_ptr<std::stringstream>& current_task,
                               std::shared_ptr<std::function<size_t()>>& callback) = 0;
    virtual size_t OpenConnection() = 0;
    virtual size_t Close() = 0;

 private:
    virtual void OnConnect_(const boost::system::error_code& error_code,
                            tcp::resolver::iterator end_point_iter,
                            std::shared_ptr<config::NetworkConnectTask>& task) = 0;
    virtual void OnReceive_(const boost::system::error_code& ErrorCode,
                            std::shared_ptr<tcp::socket>& socket) = 0;
    virtual void OnSend_(const boost::system::error_code& error_code,
                         std::shared_ptr<tcp::socket>& socket,
                         std::shared_ptr<std::string>& str_task) = 0;
    virtual void DoClose_(std::shared_ptr<tcp::socket>& socket) = 0;
};

}  // namespace network
}  // namespace client
}  // namespace failless

#endif  // LLSSCLI_NETWORK_CLIENT_INTERFACE_H_
