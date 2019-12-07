#ifndef LLSSCLIENT_CORE_CLIENT_H_
#define LLSSCLIENT_CORE_CLIENT_H_

#include <functional>

#include "llss3p/serialization/serializer.h"
#include "llsscli/config/config.h"
#include "llsscli/core/client_interface.h"
#include "llsscli/filesystem/filesystem.h"
#include "llsscli/network/network_client.h"

namespace failless {
namespace client {
namespace core {

class Client : public ClientInterface {
 public:
    explicit Client(config::ClientConfig& test_data);
    ~Client() = default;
    size_t Run() override;
    size_t ReadInput() override;

 private:
    size_t SerializeQuery_() override;
    size_t ExecQuery_() override;
    size_t ParseInput_(std::string raw_query) override;

    size_t SendToDb_() override;
    size_t SetDBKey_() override;
    size_t GetDBKey_() override;
    size_t CreateDBFolder_() override;
    size_t Register_() override;

    size_t SendToDbCallback_() override;
    size_t SetDBKeyCallback_() override;
    size_t GetDBKeyCallback_() override;
    size_t CreateDBFolderCallback_() override;
    size_t RegisterCallback_() override;

 private:
    std::unique_ptr<network::NetworkClientInterface> network_client_;
    std::unique_ptr<filesystem::FileSystemInterface> filesystem_;
    std::unique_ptr<common::serializer::SerializerInterface<config::Task>> serializer_;

    std::shared_ptr<config::NetworkConfig> net_config_;
    config::ClientConfig config_;

    std::string query_tokens_[4];
    std::shared_ptr<config::Task> current_task_;
    std::shared_ptr<std::stringstream> serialized_query_;

    bool parse_input_status_;
    bool exec_query_status_;
    bool serialize_query_status_;

    std::shared_ptr<std::function<size_t()>> send_to_db_callback_;
    std::shared_ptr<std::function<size_t()>> set_db_callback_;
    std::shared_ptr<std::function<size_t()>> get_db_callback_;
    std::shared_ptr<std::function<size_t()>> create_db_folder_callback_;
    std::shared_ptr<std::function<size_t()>> register_callback_;
};

}  // namespace core
}  // namespace client
}  // namespace failless

#endif  // LLSSCLIENT_CORE_CLIENT_H_