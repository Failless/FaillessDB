#ifndef LLSSCLIENT_CORE_CLIENT_H_
#define LLSSCLIENT_CORE_CLIENT_H_

#include "llsscli/core/client_interface.h"
#include "llsscli/filesystem/filesystem.h"
#include "llsscli/network/network_client.h"

namespace failless {
namespace client {
namespace core {

using namespace failless::client::network;

class Client : public ClientInterface {
 public:
    Client();
    ~Client();
    size_t Run() override;

 private:
    size_t SendRequestWithCB_(stringstream serialized_query, uintptr_t call_back) override;
    size_t SerializeQuery_(string query) override;
    size_t ExecQuery_(string tokens) override;
    size_t ParseInput_(string raw_query) override;
    size_t ReadInput_(int argc, char **argv) override;

    network::NetworkClient *network_client_ = nullptr;
    filesystem::FileSystem *filesystem_ = nullptr;
    string config_;
};

} // namespace core
} // namespace llsscli
} // namespace failless

#endif // LLSSCLIENT_CORE_CLIENT_H_
