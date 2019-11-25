#include "llsscli/core/client.h"

namespace failless {
namespace client {
namespace core {

Client::Client(ClientConfig test_data) {
    config_ = test_data;
    Run();
}

Client::~Client() {
    delete network_client_;
    delete filesystem_;
}

size_t Client::Run() {
    //открыть файл настроек
    //=прочитать user_name, host, port, home folder
    //запустить файловую систему (передать home folder)
    //запустить сериализатор
    //запустить сетевой клиент (передать host port)

    //вернуть статус запуска

    boost::asio::io_service io_service;
    NetworkConfig net_config;
    net_config.db_host = config_.db_host;
    net_config.db_port = config_.db_port;

    network_client_ = new NetworkClient(io_service, net_config);

    return 0;
}

size_t Client::SendRequestWithCB_(stringstream serialized_query, uintptr_t call_back) {
    return 0;
}

size_t Client::SerializeQuery_(string query) {
    return 0;
}

size_t Client::ExecQuery_(string tokens) {
    return 0;
}

size_t Client::ParseInput_(string raw_query) {
    return 0;
}

size_t Client::ReadInput_(int argc, char **argv) {
    return 0;
}

}  // namespace core
}  // namespace client
}  // namespace failless
