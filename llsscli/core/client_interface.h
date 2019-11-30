#ifndef LLSSCLI_CORE_CLIENT_INTERFACE_H_
#define LLSSCLI_CORE_CLIENT_INTERFACE_H_

#include <boost/noncopyable.hpp>
#include <string>

namespace failless {
namespace client {
namespace core {

class ClientInterface : boost::noncopyable {
 public:
    virtual ~ClientInterface() = default;
    virtual size_t Run() = 0;

    virtual size_t SendRequestWithCB_(std::stringstream serialized_query,
                                      std::uintptr_t call_back) = 0;
    virtual size_t SerializeQuery_(std::string query) = 0;
    virtual size_t ExecQuery_() = 0;
    virtual size_t ParseInput_(std::string raw_query) = 0;
    virtual size_t ReadInput_(int argc, char **argv) = 0;
};

}  // namespace core
}  // namespace client
}  // namespace failless

#endif  // LLSSCLI_CORE_CLIENT_INTERFACE_H_
