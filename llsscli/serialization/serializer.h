#include "serializer_interface.h"

namespace failless {
namespace client {
namespace serializer {

class Serializer : public SerializerInterface {
public:
    Serializer() = default;
    ~Serializer() = default;

    void Serialize(uintptr_t data) override;
    size_t Deserialize(uintptr_t data) override;

    stringstream GetSS() const;

private:
    stringstream out_buffer_;

};

};
};
};