#include <iostream>

#include "llsscli/core/client.h"
#include "llsscli/utils/str_switch.h"

// https://www.modernescpp.com/index.php/c-core-guidelines-passing-smart-pointer#h2-get-your-e-book-at-leanpub
// - про unique_ptr

namespace failless {
namespace client {
namespace core {

Client::Client(config::ClientConfig& test_data) : config_(std::move(test_data)) {
    // TODO Check inits' status
    // Init FileSystem and Serializer interfaces
    filesystem_ = std::unique_ptr<filesystem::FileSystemInterface>(new filesystem::FileSystem());
    serializer_ = std::unique_ptr<common::serializer::SerializerInterface<config::Task>>(new common::serializer::Serializer<config::Task>());

    // подумать над сверткой в 1 ф-ю
    // Init pointers to main callbacks
    send_to_db_callback_ = std::shared_ptr<std::function<size_t()>>(
        new std::function<size_t()>(std::bind(&Client::SendToDbCallback_, this)));
    set_db_callback_ = std::shared_ptr<std::function<size_t()>>(
        new std::function<size_t()>(std::bind(&Client::SetDBKeyCallback_, this)));
    get_db_callback_ = std::shared_ptr<std::function<size_t()>>(
        new std::function<size_t()>(std::bind(&Client::GetDBKeyCallback_, this)));
    create_db_folder_callback_ = std::shared_ptr<std::function<size_t()>>(
        new std::function<size_t()>(std::bind(&Client::CreateDBFolderCallback_, this)));
    register_callback_ = std::shared_ptr<std::function<size_t()>>(
        new std::function<size_t()>(std::bind(&Client::RegisterCallback_, this)));
}

size_t Client::Run() {
    // Split user request to tokens and check its status

    parse_input_status_ = ParseInput_(config_.user_request);
    if (parse_input_status_) {
        return -1;
    }

    // TODO Check init's status
    // Init base network configuration and init NetworkClient interface
    net_config_ = std::shared_ptr<config::NetworkConfig>(
        new config::NetworkConfig(config_.db_host, config_.db_port, config_.client_num));
    network_client_ =
        std::unique_ptr<network::NetworkClientInterface>(new network::NetworkClient(net_config_));

    // Execute user query and check its status
    exec_query_status_ = ExecQuery_();

    if (exec_query_status_) {
        return -1;
    }

    // Add new task for execution
    network_client_->AddUserTask(serialized_query_, send_to_db_callback_);
    // [REMOVE] It should be called during Client::Run()
    network_client_->OpenConnection();

    return 0;
}

size_t Client::SerializeQuery_() {
    // Serialize file container to stringstream and get it
//    serializer_->Serialize(current_task_);
    serializer_->Serialize(*current_task_);

    serialized_query_ = std::shared_ptr<std::stringstream>(new std::stringstream("хуй"));
    serialized_query_ = serializer_->GetOutStringStream();

    return 0;
}

size_t Client::ExecQuery_() {
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
    std::string delimiter = " ";

    size_t pos = 0;
    int i = 0;
    // 4 - max number of user req params
    while ((pos = raw_query.find(delimiter)) != std::string::npos && i < 4) {
        query_tokens_[i] = raw_query.substr(0, pos);
        std::cout << query_tokens_[i] << std::endl;
        raw_query.erase(0, pos + delimiter.length());
        ++i;
    }
    return 0;
}

size_t Client::ReadInput() {
    std::string input;
    std::cin >> input;
    while (input != "exit") {
        config_.db_host = "127.0.0.1";
        config_.db_port = "11564";
        config_.user_name = "user";
        config_.payload_dest_id = 1;
        config_.payload_key = "1";
        config_.client_num = 10;
        config_.user_request = input;
        Run();

        std::cin >> input;
    }
    return 0;
}

size_t Client::SendToDb_() {
    // Define file container and its size
    std::unique_ptr<std::vector<unsigned char>> value;
    std::streampos len;

    // Fill file container with binary bytes
    filesystem_->ReadFile(query_tokens_[1], value, len);
    std::cout << value.get()->data() << std::endl;

    // Init user Data struct
    std::unique_ptr<std::string> key(new std::string(config_.payload_key));
    std::unique_ptr<config::Data> data(new config::Data(value, len, config_.payload_dest_id, key));

    // Init user Task struct
    std::unique_ptr<std::string> user_name(new std::string(config_.user_name));
    std::unique_ptr<std::string> query(new std::string(config_.user_request));
    current_task_.reset(new config::Task(user_name, query, data));
    std::cout << current_task_.get()->payload.get()->value->data() << std::endl;

    SerializeQuery_();
    return 0;
}

size_t Client::SetDBKey_() { return 0; }
size_t Client::GetDBKey_() { return 0; }
size_t Client::CreateDBFolder_() { return 0; }
size_t Client::Register_() { return 0; }

size_t Client::SendToDbCallback_() { return 0; }
size_t Client::SetDBKeyCallback_() { return 0; }
size_t Client::GetDBKeyCallback_() { return 0; }
size_t Client::CreateDBFolderCallback_() { return 0; }
size_t Client::RegisterCallback_() { return 0; }

}  // namespace core
}  // namespace client
}  // namespace failless