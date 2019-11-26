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

    virtual void Serialize(config::Task& data) = 0;
    virtual size_t Deserialize(uintptr_t data) = 0;

    virtual std::stringstream GetSS() const = 0;

private:
    std::stringstream out_buffer_;
};

} // namespace serializer
} // namespace llsscli
} // namespace failless

#endif //FAILLESS_SERIALIZER_INTERFACE_H
