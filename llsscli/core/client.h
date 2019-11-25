#ifndef LLSSCLIENT_CORE_CLIENT_H_
#define LLSSCLIENT_CORE_CLIENT_H_

#include "llsscli/core/client_interface.h"
#include "llsscli/filesystem/filesystem.h"
#include "llsscli/serialization/serializer.h"
#include "llsscli/network/network_client.h"

namespace failless {
namespace client {
namespace core {

using namespace failless::client::network;
using namespace failless::client::filesystem;
using namespace failless::client::serializer;

struct ClientConfig {
    string user_name = "";
    string user_request = "";

    int payload_dest_id = 0;
    string payload_key = "";

    string db_host = "";
    string db_port = "";
};

class Client : public ClientInterface {
 public:
    explicit Client(ClientConfig test_data);
    ~Client() override;
    size_t Run() override;

 private:
    size_t SendRequestWithCB_(stringstream serialized_query, uintptr_t call_back) override;
    size_t SerializeQuery_(string query) override;
    size_t ExecQuery_(string tokens) override;
    size_t ParseInput_(string raw_query) override;
    size_t ReadInput_(int argc, char **argv) override;

    NetworkClient *network_client_ = nullptr;
    FileSystem *filesystem_ = nullptr;
    ClientConfig config_;
};

} // namespace core
} // namespace llsscli
} // namespace failless

#endif // LLSSCLIENT_CORE_CLIENT_H_
