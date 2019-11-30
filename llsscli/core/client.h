#ifndef LLSSCLIENT_CORE_CLIENT_H_
#define LLSSCLIENT_CORE_CLIENT_H_

#include "llsscli/config/config.h"
#include "llsscli/filesystem/filesystem.h"
#include "llsscli/network/network_client.h"
#include "llsscli/serialization/serializer.h"

#include "llsscli/core/client_interface.h"

namespace failless {
namespace client {
namespace core {

class Client : public ClientInterface {
 public:
    explicit Client(config::ClientConfig& test_data);
    ~Client() = default;
    size_t Run() override;

    size_t SendRequestWithCB_(std::stringstream serialized_query,
                              std::uintptr_t call_back) override;
    size_t SerializeQuery_(std::string query) override;
    size_t ExecQuery_() override;
    size_t ParseInput_(std::string raw_query) override;
    size_t ReadInput_(int argc, char** argv) override;

 private:
    std::unique_ptr<network::NetworkClientInterface> network_client_;
    std::unique_ptr<filesystem::FileSystemInterface> filesystem_;
    std::unique_ptr<serializer::SerializerInterface> serializer_;
    config::ClientConfig config_;

    std::string query_tokens_[4];
    std::unique_ptr<config::Task> current_task_;

    bool parse_input_status_;
    bool exec_query_status_;
    bool serialize_query_status_;
};

}  // namespace core
}  // namespace client
}  // namespace failless

#endif  // LLSSCLIENT_CORE_CLIENT_H_
