#include "llsscli/serialization/serializer_interface.h"

namespace failless {
namespace client {
namespace serializer {

class Serializer : public SerializerInterface {
public:
    Serializer() = default;
    ~Serializer() = default;

    void Serialize(config::Task& data) override;
    size_t Deserialize(uintptr_t data) override;

    std::stringstream GetSS() const override;

private:
    std::stringstream out_buffer_;

};

};
};
};