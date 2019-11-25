#ifndef FAILLESS_SERIALIZER_INTERFACE_H
#define FAILLESS_SERIALIZER_INTERFACE_H

#include <boost/noncopyable.hpp>
#include <msgpack.hpp>
#include <string>
#include <iostream>
#include <sstream>

#include "llsscli/config/config.h"

namespace failless {
namespace client {
namespace serializer {

using std::stringstream;
using std::string;

using namespace failless::client::config;

class SerializerInterface : boost::noncopyable {
public:
    SerializerInterface() = default;
    ~SerializerInterface() = default;

    virtual void Serialize(uintptr_t data) = 0;
    virtual size_t Deserialize(uintptr_t data) = 0;

    virtual stringstream GetSS() const = 0;

private:
    stringstream out_buffer_;
};

} // namespace serializer
} // namespace llsscli
} // namespace failless

#endif //FAILLESS_SERIALIZER_INTERFACE_H
