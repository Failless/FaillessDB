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

 private:
    virtual size_t SerializeQuery_() = 0;
    virtual size_t ExecQuery_() = 0;
    virtual size_t ParseInput_(std::string raw_query) = 0;

    virtual size_t SendToDb_() = 0;
    virtual size_t SetDBKey_() = 0;
    virtual size_t GetDBKey_() = 0;
    virtual size_t CreateDBFolder_() = 0;
    virtual size_t Register_() = 0;

    virtual size_t SendToDbCallback_() = 0;
    virtual size_t SetDBKeyCallback_() = 0;
    virtual size_t GetDBKeyCallback_() = 0;
    virtual size_t CreateDBFolderCallback_() = 0;
    virtual size_t RegisterCallback_() = 0;
};

}  // namespace core
}  // namespace client
}  // namespace failless

#endif  // LLSSCLI_CORE_CLIENT_INTERFACE_H_