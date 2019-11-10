#ifndef LLSSCLIENT_NETWORK_MP_CLIENT_H_
#define LLSSCLIENT_NETWORK_MP_CLIENT_H_

//#include <msgpack.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/ip/icmp.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>

namespace failless::client::mp_client {

using namespace boost::asio;

const int MAX_MSG = 1024;

class MpClientInterface {};

class MpClient : public MpClientInterface {
public:
    explicit MpClient();

    ~MpClient();
    static void Start(const ip::tcp::endpoint& ep);

    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<MpClient> ptr;

    static ptr Start(const ip::tcp::endpoint& ep, const std::string &username);
    static void Stop();
    static bool Started();

private:
    static size_t ReadComplete_(const boost::system::error_code &err, size_t bytes);

private:
    char read_buffer_[MAX_MSG]{};
    char write_buffer_[MAX_MSG]{};
};
}

#endif // LLSSCLIENT_NETWORK_MP_CLIENT_H_
