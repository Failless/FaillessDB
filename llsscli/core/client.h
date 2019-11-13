#ifndef LLSSCLIENT_CORE_CLIENT_H_
#define LLSSCLIENT_CORE_CLIENT_H_

#include "core/client_interface.h"
#include "filesystem/filesystem.h"
#include "network/mp_client.h"

namespace failless {
namespace client {
namespace core {

class Client : public ClientInterface {
public:
  Client() = default;
  Client(network::MpClient *mp_client, filesystem::FileSystem *filesystem,
         const std::string& config);
  ~Client() = default;

  size_t ReadQuery(std::string query) override;
  size_t Send() override;
  size_t Params(int argc, char **argv) override;

private:
  size_t ParseQuery_(std::string query) override;
  network::MpClient *mp_client_ = nullptr;
  filesystem::FileSystem *filesystem_ = nullptr;
  std::string config_{};
};

} // namespace core
} // namespace llsscli
} // namespace failless

#endif // LLSSCLIENT_CORE_CLIENT_H_
