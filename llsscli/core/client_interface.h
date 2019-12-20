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
    virtual size_t ReadInput() = 0;
    virtual size_t ReadNetSettings() = 0;

 private:
    virtual size_t InitNetSettings_() = 0;

    virtual size_t SerializeQuery_() = 0;
    virtual size_t ExecQuery_() = 0;
    virtual size_t ExecNet_() = 0;
    virtual size_t ParseInput_(std::string raw_query) = 0;

    virtual size_t SendToDb_() = 0;
    virtual size_t SetDBKey_() = 0;
    virtual size_t GetDBKey_() = 0;
    virtual size_t CreateDBFolder_() = 0;
    virtual size_t Connect_() = 0;
    virtual size_t Disconnect_() = 0;
    virtual size_t Register_() = 0;

    virtual size_t GeneralCallback_(char* response_data, size_t bytes_transferred) = 0;
};

}  // namespace core
}  // namespace client
}  // namespace failless

#endif  // LLSSCLI_CORE_CLIENT_INTERFACE_H_