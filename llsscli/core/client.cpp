#include <iostream>

#include "llss3p/enums/operators.h"

#include <algorithm>
#include <memory>
#include <boost/algorithm/string.hpp>
#include "llsscli/core/client.h"
#include "llsscli/utils/str_switch.h"

namespace failless {
namespace client {
namespace core {

const std::map<common::enums::response_type, std::string> kStatusMap {
    {common::enums::response_type::OK, "OK"},
    {common::enums::response_type::EXIST, "EXIST"},
    {common::enums::response_type::SERVER_ERROR, "SERVER ERROR"},
    {common::enums::response_type::NOT_DONE, "NOT DONE"},
    {common::enums::response_type::NOT_ALLOWED, "NOT ALLOWED"},
    {common::enums::response_type::NOT_FOUND, "NOT FOUND"},
    {common::enums::response_type::NOT_SET, "NOT SET"}
};


Client::Client() {
    // TODO Check inits' status
    // Init FileSystem and Serializer interfaces
    filesystem_ = std::unique_ptr<filesystem::FileSystemInterface>(new filesystem::FileSystem());
    serializer_ = std::unique_ptr<common::serializer::SerializerInterface<common::utils::Packet>>(
        new common::serializer::Serializer<common::utils::Packet>());

    general_callback_ = std::make_shared<std::function<size_t(char*, size_t)>>(
        std::bind(
            &Client::GeneralCallback_, this, std::placeholders::_1, std::placeholders::_2));
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
    return 0;
}

size_t Client::ExecNet_() {
    // Add new net task for execution
    network_client_->AddUserTask(serialized_query_, general_callback_);
    return 0;
}

size_t Client::ExecQuery_() {
    // Convert cmd name to upper case
    std::transform(query_tokens_[0].begin(), query_tokens_[0].end(), query_tokens_[0].begin(),
                   [](unsigned char c) { return std::toupper(c); });

    SWITCH(query_tokens_[0]) {
        CASE("SEND") : SendToDb_();
        break;
        CASE("GET") : GetDBKey_();
        break;
        CASE("SET") : SetDBKey_();
        break;
        CASE("CREATE") : CreateDBFolder_();
        break;
        CASE("CONNECT") : Connect_();
        break;
        CASE("DISCONNECT") : Disconnect_();
        break;
        CASE("REGISTER") : Register_();
        break;
        CASE("KILL") : Kill_();
        break;
        DEFAULT : std::cout << "[CLIENT] Error with base command!" << std::endl;
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
        config_.user_request = input;
        Run();
        std::getline(std::cin, input);
    }
    return 0;
}

size_t Client::ReadNetSettings() {
    config_.db_host = "127.0.0.1";
    config_.db_port = "11556";
    InitNetSettings_();
    return 0;
}

size_t Client::InitNetSettings_() {
    net_config_ = std::make_shared<config::NetworkConfig>(
        config_.db_host, config_.db_port);
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
    if (query_tokens_.size() == 3) {
        // Init user Data struct
        std::vector<unsigned char> value =
            std::vector<unsigned char>(query_tokens_[2].begin(), query_tokens_[2].end());
        common::utils::Data data(1, value.size(), value);

        // Init user Task struct
        current_task_.reset(new common::utils::Packet());
        current_task_->data = data;
        current_task_->command = common::enums::operators::SET;
        current_task_->ret_value = common::enums::response_type::NOT_SET;
        current_task_->request = config_.user_request;

        SerializeQuery_();

        ExecNet_();
    } else {
        std::cout << "[CLIENT] Error with SET command!" << std::endl;
    }

    return 0;
}
size_t Client::GetDBKey_() {
    // Init user Data struct
    common::utils::Data data;

    // Init user Task struct
    current_task_.reset(new common::utils::Packet());
    current_task_->data = data;
    current_task_->command = common::enums::operators::GET;
    current_task_->ret_value = common::enums::response_type::NOT_SET;
    current_task_->request = config_.user_request;

    SerializeQuery_();

    ExecNet_();
    return 0;
}

size_t Client::CreateDBFolder_() {
    std::string input;
    std::getline(std::cin, input);
    // Init user Data struct
    common::utils::Data data(std::stoi(input), 16);

    // Init user Task struct
    current_task_.reset(new common::utils::Packet());
    current_task_->data = data;
    current_task_->command = common::enums::operators::CREATE;
    current_task_->ret_value = common::enums::response_type::NOT_SET;
    current_task_->request = config_.user_request;

    SerializeQuery_();

    ExecNet_();
    return 0;
}

size_t Client::Connect_() {
    if (query_tokens_.size() == 2) {
        std::string input;
        std::getline(std::cin, input);

        // Init user Data struct
        common::utils::Data data;

        // Init user Task struct
        current_task_.reset(new common::utils::Packet());
        current_task_->login = query_tokens_[1];
        current_task_->pass = input;
        current_task_->data = data;
        current_task_->command = common::enums::operators::CONNECT;
        current_task_->ret_value = common::enums::response_type::NOT_SET;
        current_task_->request = config_.user_request;

        SerializeQuery_();

        ExecNet_();
    } else {
        std::cout << "[CONNECT] Error with command!" << std::endl;
    }
    return 0;
}
size_t Client::Disconnect_() {
    // Init user Data struct
    common::utils::Data data;

    // Init user Task struct
    current_task_.reset(new common::utils::Packet());
    current_task_->data = data;
    current_task_->command = common::enums::operators::DISCONNECT;
    current_task_->login = query_tokens_[1];
    current_task_->ret_value = common::enums::response_type::NOT_SET;
    current_task_->request = config_.user_request;

    SerializeQuery_();

    ExecNet_();
    return 0;
}

size_t Client::Register_() {
    if (query_tokens_.size() == 2) {
        std::string input;
        std::getline(std::cin, input);

        // Init user Data struct
        common::utils::Data data;

        // Init user Task struct
        current_task_.reset(new common::utils::Packet());
        current_task_->login = query_tokens_[1];
        current_task_->pass = input;
        current_task_->data = data;
        current_task_->command = common::enums::operators::REG;
        current_task_->ret_value = common::enums::response_type::NOT_SET;
        current_task_->request = config_.user_request;

        SerializeQuery_();

        ExecNet_();
    } else {
        std::cout << "[CLIENT] Error with REGISTER command!" << std::endl;
    }
    return 0;
}

size_t Client::Kill_() {
    if (query_tokens_.size() == 2) {
        std::string input;
        std::getline(std::cin, input);

        // Init user Data struct
        common::utils::Data data;
        data.folder_id = std::stoi(query_tokens_[1]);

        // Init user Task struct
        current_task_.reset(new common::utils::Packet());
        current_task_->pass = input;
        current_task_->data = data;
        current_task_->command = common::enums::operators::REG;
        current_task_->ret_value = common::enums::response_type::NOT_SET;
        current_task_->request = config_.user_request;

        SerializeQuery_();

        ExecNet_();
    } else {
        std::cout << "[CLIENT] Error with KILL command!" << std::endl;
    }
    return 0;
}

size_t Client::GeneralCallback_(char* response_data, size_t bytes_transferred) {
    std::cout << "[CALLBACK] Raw data response = " << std::endl;
    serializer_->Deserialize(reinterpret_cast<char*>(response_data), bytes_transferred);
    response_task_ = serializer_->GetInConfig();
    try {
        std::cout << "[CALLBACK] "
                  << kStatusMap.at(
                         static_cast<common::enums::response_type>(response_task_->ret_value))
                  << std::endl;
    } catch (std::out_of_range& e) {
        std::cerr << "UNKNOWN RETURN STATUS: " << response_task_->ret_value << std::endl;
    }
    return 0;
}

}  // namespace core
}  // namespace client
}  // namespace failless