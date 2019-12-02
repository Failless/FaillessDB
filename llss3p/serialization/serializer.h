#include <iostream>
#include "llss3p/serialization/serializer_interface.h"

namespace failless {
namespace common {
namespace serializer {

template <class T>
class Serializer : public SerializerInterface<T> {
 public:
    Serializer() = default;
    ~Serializer() = default;

    size_t Serialize(std::unique_ptr<T>& data) override;
    size_t Deserialize(std::unique_ptr<std::stringstream>& current_task) override;

    std::unique_ptr<std::stringstream>& GetOutStringStream() override;
    std::unique_ptr<T>& GetInConfig() override;

 private:
    std::unique_ptr<std::stringstream> out_buf_;
    std::unique_ptr<T> in_buf_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                              ///
///                            Implementation                                                    ///
///   Because different objects at the present time should be serialized I rewrote this part on  ///
///   templates and this is the reason why I have implemented methods here (With templates       ///
///   objects we have only two ways:                                                             ///
///    * put its implementation in the header file and templates will generate for such types    ///
///    that we put in <> when we create serializer object and include header                     ///
///    * create an implementation in CPP files, but forced generate realization for using data   ///
///    types. For example: template <common::Data> or template <client::Task>)                   ///
///                                                                                              ///
////////////////////////////////////////////////////////////////////////////////////////////////////

template <class T>
size_t Serializer<T>::Serialize(std::unique_ptr<T>& data) {
    out_buf_ = std::make_unique<std::stringstream>();
    msgpack::pack(*out_buf_, *(data.get()));
    return 0;
}

template <class T>
size_t Serializer<T>::Deserialize(std::unique_ptr<std::stringstream>& current_task) {
    // deserialize
    msgpack::object_handle oh =
        msgpack::unpack(out_buf_.get()->str().data(), out_buf_.get()->str().size());
    msgpack::object deserialized = oh.get();
    std::cout << deserialized << std::endl;

    in_buf_.reset(new T());
    deserialized.convert(*(in_buf_.get()));

    std::stringstream ss;
    ss << deserialized;

    std::cout << in_buf_.get()->payload.get()->value.get()->data() << std::endl;
    std::cout << ss.str().c_str() << std::endl;
    return 0;
}

template <class T>
std::unique_ptr<std::stringstream>& Serializer<T>::GetOutStringStream() {
    return out_buf_;
}

template <class T>
std::unique_ptr<T>& Serializer<T>::GetInConfig() {
    return in_buf_;
}

};  // namespace serializer
};  // namespace common
};  // namespace failless