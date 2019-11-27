#ifndef FAILLESS_SERIALIZER_INTERFACE_H
#define FAILLESS_SERIALIZER_INTERFACE_H

#include <boost/noncopyable.hpp>
#include <sstream>

#include "llsscli/config/config.h"

namespace failless {
namespace client {
namespace serializer {

class SerializerInterface : boost::noncopyable {
public:
    virtual ~SerializerInterface() = default;

    virtual size_t Serialize(std::unique_ptr< config::Task >& current_task) = 0;
    virtual size_t Deserialize(std::unique_ptr< std::stringstream >& current_task) = 0;

    virtual std::unique_ptr< std::stringstream >& GetOutStringStream() = 0;
    virtual std::unique_ptr< config::Task >& GetInConfig() = 0;

};

} // namespace serializer
} // namespace llsscli
} // namespace failless

#endif //FAILLESS_SERIALIZER_INTERFACE_H
