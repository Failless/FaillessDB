#include "llsscli/serialization/serializer_interface.h"

namespace failless {
namespace client {
namespace serializer {

class Serializer : public SerializerInterface {
public:
    Serializer() = default;
    ~Serializer() = default;

    size_t Serialize(std::shared_ptr< config::Task >& current_task) override;
    size_t Deserialize(std::shared_ptr< std::stringstream >& current_task) override;

    std::shared_ptr< std::stringstream >& GetOutStringStream() override;
    std::shared_ptr< config::Task >& GetInConfig() override;

private:
    std::shared_ptr< std::stringstream > out_buf_;
    std::shared_ptr< config::Task > in_buf_;
};

};
};
};