#ifndef LLSSCLIENT_CORE_CLIENT_H_
#define LLSSCLIENT_CORE_CLIENT_H_

#include "llsscli/core/client_interface.h"

namespace failless {
namespace client {
namespace core {

class Client : public ClientInterface {
 public:
    explicit Client(ClientConfig& test_data);
    ~Client() override;
    size_t Run() override;

 private:
    size_t SendRequestWithCB_(stringstream serialized_query, uintptr_t call_back) override;
    size_t SerializeQuery_(string query) override;
    size_t ExecQuery_() override;
    size_t ParseInput_(string raw_query) override;
    size_t ReadInput_(int argc, char **argv) override;

    unique_ptr< NetworkClient > network_client_;
    unique_ptr< FileSystem > filesystem_;
    unique_ptr< Serializer > serializer_;
    ClientConfig config_;

    string query_tokens_[4];
    Task current_task_;

    bool parse_input_status_;
    bool exec_query_status_;
    bool serialize_query_status_;
};

} // namespace core
} // namespace llsscli
} // namespace failless

#endif // LLSSCLIENT_CORE_CLIENT_H_
