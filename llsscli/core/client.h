#ifndef LLSSCLIENT_CORE_CLIENT_H_
#define LLSSCLIENT_CORE_CLIENT_H_

#include <functional>

#include "llss3p/serialization/serializer.h"
#include "llss3p/utils/data.h"
#include "llss3p/utils/packet.h"
#include "llsscli/config/config.h"
#include "llsscli/core/client_interface.h"
#include "llsscli/filesystem/filesystem.h"
#include "llsscli/network/network_client.h"

namespace failless {
namespace client {
namespace core {

class Client : public ClientInterface {
 public:
    explicit Client();
    ~Client() = default;
    size_t Run() override;
    size_t ReadInput() override;
    size_t ReadNetSettings() override;

 private:
    size_t InitNetSettings_() override;

    size_t SerializeQuery_() override;
    size_t ExecQuery_() override;
    size_t ExecNet_() override;
    size_t ParseInput_(std::string raw_query) override;

    size_t SendToDb_() override;
    size_t SetDBKey_() override;
    size_t GetDBKey_() override;
    size_t CreateDBFolder_() override;
    size_t Connect_() override;
    size_t Disconnect_() override;
    size_t Register_() override;

    size_t GeneralCallback_(char* response_data, size_t bytes_transferred) override;

 private:
    std::unique_ptr<network::NetworkClientInterface> network_client_;
    std::unique_ptr<filesystem::FileSystemInterface> filesystem_;
    std::unique_ptr<common::serializer::SerializerInterface<common::utils::Packet>> serializer_;

    std::shared_ptr<config::NetworkConfig> net_config_;
    config::ClientConfig config_;

    std::vector<std::string> query_tokens_;
    std::shared_ptr<common::utils::Packet> current_task_;
    std::shared_ptr<std::stringstream> serialized_query_;

    bool parse_input_status_;
    bool exec_query_status_;
    bool serialize_query_status_;

    std::shared_ptr<std::function<size_t(char*, size_t)>> general_callback_;
    std::shared_ptr<common::utils::Packet> response_task_;

    std::map<common::enums::response_type, std::string> status_map_{
        {common::enums::response_type::OK, "OK"},
        {common::enums::response_type::SERVER_ERROR, "ERROR"}};
};

}  // namespace core
}  // namespace client
}  // namespace failless

#endif  // LLSSCLIENT_CORE_CLIENT_H_