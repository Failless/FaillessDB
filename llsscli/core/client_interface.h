#ifndef LLSSCLI_CORE_CLIENT_INTERFACE_H_
#define LLSSCLI_CORE_CLIENT_INTERFACE_H_

#include <boost/noncopyable.hpp>
#include <string>

#include "llsscli/filesystem/filesystem.h"
#include "llsscli/serialization/serializer.h"
#include "llsscli/network/network_client.h"
#include "llsscli/config/config.h"

namespace failless {
namespace client {
namespace core {

using std::string;
using std::stringstream;
using std::unique_ptr;

using namespace failless::client::network;
using namespace failless::client::filesystem;
using namespace failless::client::serializer;
using namespace failless::client::config;

class ClientInterface : boost::noncopyable {
public:
    virtual ~ClientInterface() = default;
    virtual size_t Run() = 0;

private:
    virtual size_t SendRequestWithCB_(stringstream serialized_query, uintptr_t call_back) = 0;
    virtual size_t SerializeQuery_(string query) = 0;
    virtual size_t ExecQuery_() = 0;
    virtual size_t ParseInput_(string raw_query) = 0;
    virtual size_t ReadInput_(int argc, char **argv) = 0;
};

}  // namespace core
}  // namespace client
}  // namespace failless

#endif  // LLSSCLI_CORE_CLIENT_INTERFACE_H_
