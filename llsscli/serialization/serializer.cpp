#include "llsscli/serialization/serializer.h"

namespace failless {
namespace client {
namespace serializer {

void Serializer::Serialize(Task& data) {

}

size_t Serializer::Deserialize(uintptr_t data) {
    return 0;
}

stringstream Serializer::GetSS() const {
    return std::stringstream();
}

} // namespace serializer
} // namespace llsscli
} // namespace failless
