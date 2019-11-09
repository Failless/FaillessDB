#ifndef LLSSCLIENT_NETWORK_MP_CLIENT_H_
#define LLSSCLIENT_NETWORK_MP_CLIENT_H_

namespace mp_client_network {

    #include <msgpack.hpp>
    #include <boost/asio/io_service.hpp>
    #include <boost/asio/deadline_timer.hpp>
    #include <boost/asio/ip/icmp.hpp>
    #include <boost/asio/ip/tcp.hpp>
    #include <boost/enable_shared_from_this.hpp>
    #include <boost/noncopyable.hpp>

    using namespace boost::asio;

    class mp_client_interface {
    public:

    private:

    };

    class mp_client
            : public mp_client_interface, public boost::enable_shared_from_this<mp_client>, boost::noncopyable {
    public:
        mp_client(const std::string &username);
        ~mp_client();
        void start(ip::tcp::endpoint ep);

        typedef boost::system::error_code error_code;
        typedef boost::shared_ptr<mp_client> ptr;

        static ptr start(ip::tcp::endpoint ep, const std::string &username);
        void stop();
        bool started();

    private:
        size_t read_complete(const boost::system::error_code &err, size_t bytes);

    private:
        ip::tcp::socket sock_;
        enum {
            max_msg = 1024
        };
        char read_buffer_[max_msg];
        char write_buffer_[max_msg];
        bool started_;
        std::string username_;
        deadline_timer timer_;
    };
}

#endif // LLSSCLIENT_NETWORK_MP_CLIENT_H_
