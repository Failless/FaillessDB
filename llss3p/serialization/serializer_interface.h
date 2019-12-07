#ifndef FAILLESS_SERIALIZER_INTERFACE_H
#define FAILLESS_SERIALIZER_INTERFACE_H

#include <boost/noncopyable.hpp>
#include <sstream>
#include "llsscli/config/config.h"

namespace failless {
namespace common {
namespace serializer {

template <class T>
class SerializerInterface : boost::noncopyable {
 public:
    virtual ~SerializerInterface() = default;

    virtual size_t Serialize(T& data) = 0;
    virtual T Deserialize(char* data, size_t size) = 0;

    virtual std::stringstream* GetOutStringStream() = 0;
    virtual std::shared_ptr<T> GetInConfig() = 0;
};

}  // namespace serializer
}  // namespace common
}  // namespace failless

#endif  // FAILLESS_SERIALIZER_INTERFACE_H
