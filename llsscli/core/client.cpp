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

    filesystem_ = new FileSystem();
    serializer_ = new Serializer();

    parse_input_status_ = ParseInput_(config_.user_request);
    if (parse_input_status_)
        return -1;

    exec_query_status_ = ExecQuery_();
    if (exec_query_status_)
        return -1;

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

size_t Client::ExecQuery_() {
    if (query_tokens_[0] == "SEND") {
        filesystem_->ReadFile(query_tokens_[1]);
        std::vector<uint8_t>* payload = filesystem_->GetPayload();

        current_task_.client_id = config_.user_name;
        current_task_.query = config_.user_request;
        current_task_.payload.value = payload;
        current_task_.payload.size = 0;
        current_task_.payload.folder_id = 0;
        current_task_.payload.key = "";

        serializer_->Serialize(current_task_);
    } else if (query_tokens_[0] == "GET") {
        // TODO
    }
    return 0;
}

size_t Client::ParseInput_(string raw_query) {
    string delimiter = " ";

    size_t pos = 0;
    int i = 0;
    while ((pos = raw_query.find(delimiter)) != std::string::npos && i < 4) {
        query_tokens_[i] = raw_query.substr(0, pos);
        std::cout << query_tokens_[i] << std::endl;
        raw_query.erase(0, pos + delimiter.length());
        i++;
    }
    std::cout << raw_query << std::endl;
    return 0;
}

// useless for now
size_t Client::ReadInput_(int argc, char **argv) {
    return 0;
}

}  // namespace core
}  // namespace client
}  // namespace failless
