#include "llssclient/network/mp_client.h"

#include <utility>

failless::client::mp_client::MpClient::MpClient() {

}

failless::client::mp_client::MpClient::~MpClient() = default;

void failless::client::mp_client::MpClient::Stop() {

}

size_t failless::client::mp_client::MpClient::ReadComplete_(const boost::system::error_code &err, size_t bytes) {
    return 0;
}

bool failless::client::mp_client::MpClient::Started() {
    return false;
}

void failless::client::mp_client::MpClient::Start(const boost::asio::ip::tcp::endpoint& ep) {

}

failless::client::mp_client::MpClient::ptr
failless::client::mp_client::MpClient::Start(const boost::asio::ip::tcp::endpoint& ep, const std::string &username) {
    return failless::client::mp_client::MpClient::ptr();
}
