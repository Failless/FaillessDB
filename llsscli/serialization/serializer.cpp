#include <iostream>
#include <vector>

#include "llsscli/serialization/serializer.h"

// https://github.com/msgpack/msgpack-c/wiki/v2_0_cpp_adaptor - docs
// https://qiita.com/koorinonaka/items/e99c70f029c986d35bb8

namespace failless {
namespace client {
namespace serializer {

size_t Serializer::Serialize(std::unique_ptr< config::Task >& current_task) {
    out_buf_.reset(new std::stringstream());
    msgpack::pack(*out_buf_.get(), *(current_task.get()));
    return 0;
}

size_t Serializer::Deserialize(std::unique_ptr< std::stringstream >& current_task) {
    // deserialize
    msgpack::object_handle oh = msgpack::unpack( out_buf_.get()->str().data(), out_buf_.get()->str().size() );
    msgpack::object deserialized = oh.get();
    std::cout << deserialized << std::endl;

    in_buf_.reset(new config::Task());
    deserialized.convert( *( in_buf_.get() ) );

    std::stringstream ss;
    ss << deserialized;

    std::cout<<in_buf_.get()->payload.get()->value.get()->data()<<std::endl;
    std::cout<<ss.str().c_str()<<std::endl;
    return 0;
}

std::unique_ptr< std::stringstream >& Serializer::GetOutStringStream() {
    return out_buf_;
}

std::unique_ptr< config::Task >& Serializer::GetInConfig() {
    return in_buf_;
}

} // namespace serializer
} // namespace llsscli
} // namespace failless