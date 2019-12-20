#include <iostream>

#include "llss3p/enums/operators.h"

#include <boost/algorithm/string.hpp>
#include "llsscli/core/client.h"
#include "llsscli/utils/str_switch.h"

namespace failless {
namespace client {
namespace core {

Client::Client() {
    // TODO Check inits' status
    // Init FileSystem and Serializer interfaces
    filesystem_ = std::unique_ptr<filesystem::FileSystemInterface>(new filesystem::FileSystem());
    serializer_ = std::unique_ptr<common::serializer::SerializerInterface<common::utils::Packet>>(
        new common::serializer::Serializer<common::utils::Packet>());

    general_callback_ = std::shared_ptr<std::function<size_t(std::shared_ptr<std::vector<unsigned char>>&)>>(
        new std::function<size_t(std::shared_ptr<std::vector<unsigned char>>&)>(std::bind(&Client::GeneralCallback_, this, std::placeholders::_1)));
}

size_t Client::Run() {
    // Split user request to tokens and check its status
    parse_input_status_ = ParseInput_(config_.user_request);
    if (parse_input_status_) {
        return -1;
    }

    // Execute user query and check its status
    exec_query_status_ = ExecQuery_();

    if (exec_query_status_) {
        return -1;
    }
    return 0;
}

size_t Client::SerializeQuery_() {
    // Serialize file container to stringstream and get it
    serializer_->Serialize(*current_task_);
    serialized_query_ = serializer_->GetOutStringStream();
    std::cout << "serialized_query_=" << serialized_query_->str() << std::endl;

    return 0;
}

size_t Client::ExecNet_() {
    // Add new net task for execution
    network_client_->AddUserTask(serialized_query_, general_callback_);
    return 0;
}

size_t Client::ExecQuery_() {
    // Convert cmd name to upper case
    std::for_each(query_tokens_[0].begin(), query_tokens_[0].end(),
                  [](char& c) { c = std::toupper(c); });

    SWITCH(query_tokens_[0]) {
        CASE("SEND") : SendToDb_();
        break;
        CASE("GET") : GetDBKey_();
        break;
        CASE("SET") : SetDBKey_();
        break;
        CASE("CREATE") : CreateDBFolder_();
        break;
        CASE("REGISTER") : Register_();
        break;
    }
    return 0;
}

size_t Client::ParseInput_(std::string raw_query) {
    query_tokens_.clear();
    boost::split(query_tokens_, raw_query, [](char c) { return c == ' '; });
    return 0;
}

size_t Client::ReadInput() {
    std::string input;
    std::getline(std::cin, input);
    while (input != "exit") {
        config_.user_name = "user";
        config_.payload_dest_id = 1;
        config_.payload_key = "payload_key";
        config_.user_request = input;
        Run();

        std::getline(std::cin, input);
    }
    return 0;
}

size_t Client::ReadNetSettings() {
    //    std::string input;
    //    std::getline(std::cin, input);
    config_.db_host = "127.0.0.1";
    config_.db_port = "11556";
    InitNetSettings_();
    return 0;
}

size_t Client::InitNetSettings_() {
    net_config_ = std::shared_ptr<config::NetworkConfig>(
        new config::NetworkConfig(config_.db_host, config_.db_port));
    network_client_ =
        std::unique_ptr<network::NetworkClientInterface>(new network::NetworkClient(net_config_));
    return 0;
}

size_t Client::SendToDb_() {
    //    // Define file container and its size
    //    std::unique_ptr<std::vector<unsigned char>> value;
    //    std::streampos len;
    //
    //    // Fill file container with binary bytes
    //    filesystem_->ReadFile(query_tokens_[1], value, len);
    //    std::cout << value->data() << std::endl;
    //
    //    // Init user Data struct
    //    std::unique_ptr<std::string> key(new std::string(config_.payload_key));
    //    std::unique_ptr<config::Data> data(new config::Data(value, len, config_.payload_dest_id,
    //    key));
    //
    //    // Init user Task struct
    //    std::unique_ptr<std::string> user_name(new std::string(config_.user_name));
    //    std::unique_ptr<std::string> query(new std::string(config_.user_request));
    //    current_task_.reset(new config::Task(user_name, query, data));
    //    std::cout << current_task_->payload->value->data() << std::endl;
    //
    //    SerializeQuery_();
    return 0;
}

size_t Client::SetDBKey_() {
    // Init user Data struct
    query_tokens_[2] = "\n";
    std::vector<unsigned char> value = std::vector<unsigned char>(query_tokens_[2].begin(),
                                                         query_tokens_[2].end());
    common::utils::Data data(1, value.size(), value);

    // Init user Task struct
    current_task_.reset(new common::utils::Packet());
    current_task_->data = data;
    current_task_->command = common::enums::operators::SET;
    current_task_->login = "user";
    current_task_->pass = "pass";
    current_task_->ret_value = common::enums::response_type::NOT_SET;
    current_task_->request = config_.user_request;

    SerializeQuery_();

    ExecNet_();

    return 0;
}
size_t Client::GetDBKey_() {
    // Init user Data struct
    // ДЛЯ СЛУЧАЕ ЭХО ОТВЕТА В КОЛБЕК ПУСТОЙ VALUE РУИНИТ MSGPACK ПРИ ДЕСЕРИАЛИЗАЦИИ
    common::utils::Data data(1, 228);

    // Init user Task struct
    current_task_.reset(new common::utils::Packet());
    current_task_->data = data;
    current_task_->command = common::enums::operators::GET;
    current_task_->login = "user";
    current_task_->pass = "pass";
    current_task_->ret_value = common::enums::response_type::NOT_SET;
    current_task_->request = config_.user_request;

    SerializeQuery_();

    ExecNet_();
    return 0;
}

size_t Client::CreateDBFolder_() { return 0; }
size_t Client::Register_() { return 0; }

size_t Client::GeneralCallback_(std::shared_ptr<std::vector<unsigned char>>& content_vector) {
    response_ = content_vector;
    serializer_->Deserialize(reinterpret_cast<char*>(content_vector->data()), content_vector->size());
    response_task_ = serializer_->GetInConfig();
    std::cout << "response_task_=" << response_task_->login << std::endl;
    return 0;
}

}  // namespace core
}  // namespace client
}  // namespace failless