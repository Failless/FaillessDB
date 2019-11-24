#include "llsscli/network/network_client.h"

namespace failless {
namespace client {
namespace network {

NetworkClient::NetworkClient(boost::asio::io_service& io_service, tcp::resolver::iterator end_point_iter)
        : io_service_(io_service), socket_(io_service), send_buffer_("")
{
    tcp::endpoint end_point = *end_point_iter;

    socket_.async_connect(end_point,
                          boost::bind(&NetworkClient::OnConnect_, this, boost::asio::placeholders::error, ++end_point_iter));
}

NetworkClient::~NetworkClient(){}

void NetworkClient::Close()
{
    io_service_.post(
            boost::bind(&NetworkClient::DoClose_, this));
}
void NetworkClient::OnConnect_(const boost::system::error_code& ErrorCode, tcp::resolver::iterator EndPointIter)
{
    cout << "OnConnect..." << endl;
    if (ErrorCode.value() == boost::system::errc::success)
    {
        cin >> send_buffer_;
        cout << "Entered: " << send_buffer_ << endl;

        socket_.async_write_some(
                boost::asio::buffer(send_buffer_, send_buffer_.length()+1),
                boost::bind(&NetworkClient::OnSend_, this,
                            boost::asio::placeholders::error));
    }
    else if (EndPointIter != tcp::resolver::iterator())
    {
        socket_.close();
        tcp::endpoint EndPoint = *EndPointIter;

        socket_.async_connect(EndPoint,
                              boost::bind(&NetworkClient::OnConnect_, this, boost::asio::placeholders::error, ++EndPointIter));
    }
}

void NetworkClient::OnReceive_(const boost::system::error_code& ErrorCode)
{
    cout << "receiving..." << endl;
    if (ErrorCode.value() == boost::system::errc::success)
    {
        cout << recieve_buffer_ << endl;

//        socket_.async_read_some(boost::asio::buffer(recieve_buffer_, buflen_),
//                               boost::bind(&NetworkClient::OnReceive_, this, boost::asio::placeholders::error));
    }
    else
    {
        cout << "ERROR! OnReceive..." << endl;
        DoClose_();
    }
}

void NetworkClient::OnSend_(const boost::system::error_code& error_code)
{
    cout << "sending..." << endl;
    if (!error_code)
    {
        cout << "\""<< send_buffer_ <<"\" has been sent" << endl;
        send_buffer_ = "";

        socket_.async_receive(boost::asio::buffer(recieve_buffer_, buflen_),
                              boost::bind(&NetworkClient::OnReceive_, this, boost::asio::placeholders::error));
    }
    else
    {
        cout << "OnSend closing" << endl;
        DoClose_();
    }

}

void NetworkClient::DoClose_()
{
    socket_.close();
}

}  // namespace network
}  // namespace client
}  // namespace failless
