#include "llsscli/core/client.h"
#include <iostream>
#include <memory>

// https://www.modernescpp.com/index.php/c-core-guidelines-passing-smart-pointer#h2-get-your-e-book-at-leanpub
// - про unique_ptr

namespace failless {
namespace client {
namespace core {

Client::Client(config::ClientConfig& test_data) : config_(std::move(test_data)) {}

size_t Client::Run() {
    //открыть файл настроек
    //=прочитать user_name, host, port, home folder
    //запустить файловую систему (передать home folder)
    //запустить сериализатор
    //запустить сетевой клиент (передать host port)

    //вернуть статус запуска

    filesystem_ = std::make_unique<filesystem::FileSystem>();
    serializer_ = std::make_unique<common::serializer::Serializer<config::Task>>();

    parse_input_status_ = ParseInput_(config_.user_request);
    if (parse_input_status_) {
        return -1;
    }

    exec_query_status_ = ExecQuery_();
    if (exec_query_status_) {
        return -1;
    }

    boost::asio::io_service io_service;
    config::NetworkConfig net_config(config_.db_host, config_.db_port);

    network_client_ = std::make_unique<network::NetworkClient>(io_service, net_config);

    return 0;
}

size_t Client::SendRequestWithCB_(std::stringstream serialized_query, std::uintptr_t call_back) {
    return 0;
}

size_t Client::SerializeQuery_(std::string query) { return 0; }

size_t Client::ExecQuery_() {
    if (query_tokens_[0] == "SEND") {
        std::unique_ptr<std::vector<unsigned char>> value;
        std::streampos len;

        filesystem_->ReadFile(query_tokens_[1], value, len);

        std::cout << value->data() << std::endl;

        std::unique_ptr<std::string> key(new std::string(""));
        std::unique_ptr<config::Data> data(new config::Data(value, len, 0, key));

        std::unique_ptr<std::string> user_name(new std::string(config_.user_name));
        std::unique_ptr<std::string> query(new std::string(config_.user_request));

        current_task_ = std::make_unique<config::Task>(user_name, query, data);

        std::cout << current_task_->payload->value->data() << std::endl;

        serializer_->Serialize(current_task_);
    } else if (query_tokens_[0] == "GET") {
        // TODO
    }
    return 0;
}

size_t Client::ParseInput_(std::string raw_query) {
    std::string delimiter = " ";

    size_t pos = 0;
    int i = 0;
    while ((pos = raw_query.find(delimiter)) != std::string::npos && i < 4) {
        query_tokens_[i] = raw_query.substr(0, pos);
        std::cout << query_tokens_[i] << std::endl;
        raw_query.erase(0, pos + delimiter.length());
        ++i;
    }
    std::cout << raw_query << std::endl;
    return 0;
}

// useless for now
size_t Client::ReadInput_(int argc, char** argv) { return 0; }

}  // namespace core
}  // namespace client
}  // namespace failless