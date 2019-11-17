#include "llsscli/core/client.h"

namespace failless {
namespace client {
namespace core {

Client::Client(network::MpClient *mp_client, filesystem::FileSystem *filesystem,
               const std::string &config) {}

size_t Client::ReadQuery(std::string query) { return 0; }

size_t Client::Send() { return 0; }

size_t Client::ParseQuery_(std::string query) { return 0; }

size_t Client::Params(int argc, char **argv) { return 0; }

}  // namespace core
}  // namespace client
}  // namespace failless
