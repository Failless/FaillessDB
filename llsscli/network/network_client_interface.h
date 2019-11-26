#ifndef LLSSCLI_NETWORK_CLIENT_INTERFACE_H_
#define LLSSCLI_NETWORK_CLIENT_INTERFACE_H_

//https://stackoverflow.com/questions/12990840/boost-async-tcp-client
//https://www.codeproject.com/Articles/1264257/Socket-Programming-in-Cplusplus-using-boost-asio-T

#include <boost/core/noncopyable.hpp>
#include <iostream>
#include <boost/asio/ip/tcp.hpp>
#include <string>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <exception>
#include "llsscli/config/config.h"

namespace failless {
namespace client {
namespace network {

using boost::asio::ip::tcp;

class NetworkClientInterface : boost::noncopyable {
public:
    virtual ~NetworkClientInterface() = default;

    virtual void Close() = 0;

private:
    virtual void OnConnect_(const boost::system::error_code& error_code, tcp::resolver::iterator end_point_iter) = 0;
    virtual void OnReceive_(const boost::system::error_code& error_code) = 0;
    virtual void OnSend_(const boost::system::error_code& error_code) = 0;
    virtual void DoClose_() = 0;
};

} // namespace network
} // namespace llsscli
} // namespace failless

#endif // LLSSCLI_NETWORK_CLIENT_INTERFACE_H_
