#ifndef FAILLESS_IAUTHORIZATION_H
#define FAILLESS_IAUTHORIZATION_H

#include <boost/noncopyable.hpp>
#include <map>
#include <string>

// path to file with users data
const std::string kUsersPath = "/tmp/users.conf";

namespace failless {
namespace db {
namespace auth {

class IAuthorization : boost::noncopyable {
 public:
    virtual bool Registration(const std::string &login, const std::string &pass, int folder_id) = 0;

    virtual bool IsAuth(const std::string &login, const std::string &pass, int table_id) = 0;

    virtual bool RemoveUser(const std::string &login, const std::string &pass) = 0;

    virtual ~IAuthorization() = default;
};

}  // namespace auth
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_IAUTHORIZATION_H
