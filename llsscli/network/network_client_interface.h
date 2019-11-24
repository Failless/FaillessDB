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

namespace failless {
namespace client {
namespace network {

using boost::asio::ip::tcp;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::exception;
using namespace boost;

class NetworkClientInterface : boost::noncopyable {
public:
    NetworkClientInterface() = default;
    ~NetworkClientInterface() = default;

    virtual void Close() = 0;

private:
    virtual void OnConnect_(const boost::system::error_code& error_code, tcp::resolver::iterator end_point_iter) = 0;
    virtual void OnReceive_(const boost::system::error_code& error_code) = 0;
    virtual void OnSend_(const boost::system::error_code& error_code) = 0;
    virtual void DoClose_() = 0;
};

//class TCPClient
//{
//public:
//    TCPClient(boost::asio::io_service& IO_Service, tcp::resolver::iterator EndPointIter);
//    void Close();
//
//private:
//    boost::asio::io_service& m_IOService;
//    tcp::socket m_Socket;
//
//    string m_SendBuffer;
//    static const size_t m_BufLen = 100;
//    char m_RecieveBuffer[m_BufLen*2];
//
//    void OnConnect(const boost::system::error_code& ErrorCode, tcp::resolver::iterator EndPointIter);
//    void OnReceive(const boost::system::error_code& ErrorCode);
//    void OnSend(const boost::system::error_code& ErrorCode);
//    void DoClose();
//};
//
//TCPClient::TCPClient(boost::asio::io_service& IO_Service, tcp::resolver::iterator EndPointIter)
//        : m_IOService(IO_Service), m_Socket(IO_Service), m_SendBuffer("")
//{
//    tcp::endpoint EndPoint = *EndPointIter;
//
//    m_Socket.async_connect(EndPoint,
//                           boost::bind(&TCPClient::OnConnect, this, boost::asio::placeholders::error, ++EndPointIter));
//}
//
//void TCPClient::Close()
//{
//    m_IOService.post(
//            boost::bind(&TCPClient::DoClose, this));
//}
//void TCPClient::OnConnect(const boost::system::error_code& ErrorCode, tcp::resolver::iterator EndPointIter)
//{
//    cout << "OnConnect..." << endl;
//    if (ErrorCode.value() == boost::system::errc::success)
//    {
//        cin >> m_SendBuffer;
//        cout << "Entered: " << m_SendBuffer << endl;
//
//        m_Socket.async_write_some(
//                boost::asio::buffer(m_SendBuffer, m_SendBuffer.length()+1),
//                boost::bind(&TCPClient::OnSend, this,
//                            boost::asio::placeholders::error));
//    }
//    else if (EndPointIter != tcp::resolver::iterator())
//    {
//        m_Socket.close();
//        tcp::endpoint EndPoint = *EndPointIter;
//
//        m_Socket.async_connect(EndPoint,
//                               boost::bind(&TCPClient::OnConnect, this, boost::asio::placeholders::error, ++EndPointIter));
//    }
//}
//
//void TCPClient::OnReceive(const boost::system::error_code& ErrorCode)
//{
//    cout << "receiving..." << endl;
//    if (ErrorCode.value() == boost::system::errc::success)
//    {
//        cout << m_RecieveBuffer << endl;
//
////        m_Socket.async_read_some(boost::asio::buffer(m_RecieveBuffer, m_BufLen),
////                               boost::bind(&TCPClient::OnReceive, this, boost::asio::placeholders::error));
//    }
//    else
//    {
//        cout << "ERROR! OnReceive..." << endl;
//        DoClose();
//    }
//}
//
//void TCPClient::OnSend(const boost::system::error_code& ErrorCode)
//{
//    cout << "sending..." << endl;
//    if (!ErrorCode)
//    {
//        cout << "\""<< m_SendBuffer <<"\" has been sent" << endl;
//        m_SendBuffer = "";
//
//        m_Socket.async_receive(boost::asio::buffer(m_RecieveBuffer, m_BufLen),
//                               boost::bind(&TCPClient::OnReceive, this, boost::asio::placeholders::error));
//    }
//    else
//    {
//        cout << "OnSend closing" << endl;
//        DoClose();
//    }
//
//}
//
//void TCPClient::DoClose()
//{
//    m_Socket.close();
//}

} // namespace network
} // namespace llsscli
} // namespace failless

#endif // LLSSCLI_NETWORK_CLIENT_INTERFACE_H_
