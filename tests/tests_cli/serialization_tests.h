#ifndef FAILLESS_TESTS_TESTS_CLI_SERIALIZATION_TESTS_H_
#define FAILLESS_TESTS_TESTS_CLI_SERIALIZATION_TESTS_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <cstring>
#include <cstdio>
#include <memory>
#include "llss3p/serialization/serializer.h"
#include "llss3p/utils/packet.h"

namespace failless {
namespace client {
namespace tests {

using ::testing::_;
using ::testing::AtLeast;

namespace utl = common::utils;
namespace srz = common::serializer;

class SeralizerImpl : public ::testing::Test {
 public:
    void SetUp() {
        data_ = utl::Data(-1, 0);
        packet_.login = "login";
        packet_.pass = "password";
        packet_.ret_value = common::enums::response_type::NOT_SET;
        packet_.command = common::enums::operators::REG;
        packet_.data = data_;
        serializer_ = std::make_unique<srz::Serializer<utl::Packet>>();
        bytes_ = nullptr;
        size_ = 0;
    }
    utl::Packet packet_;
    utl::Data data_;
    std::unique_ptr<srz::SerializerInterface<utl::Packet>> serializer_;
    char *bytes_{};
    size_t size_{};
};

TEST_F(SeralizerImpl, SeralizeDeserialize) {
    EXPECT_EQ(serializer_->Serialize(packet_), 0);
    auto *stream = serializer_->GetOutStringStream();
    std::cout << stream->str() << std::endl;
    stream->seekp(0, std::ios::end);
    std::stringstream::pos_type offset = stream->tellp();
    int real_size = static_cast<int>(offset);
    bytes_ = new char[real_size];
    memcpy(bytes_, stream->str().c_str(), real_size);
    size_ = real_size;
    auto packet_out = serializer_->Deserialize(bytes_, size_);
    EXPECT_EQ(packet_out.command, packet_.command);
    EXPECT_EQ(packet_out.ret_value, packet_.ret_value);
    EXPECT_EQ(packet_out.pass, packet_.pass);
    EXPECT_EQ(packet_out.login, packet_.login);
    EXPECT_EQ(packet_out.data.folder_id, packet_.data.folder_id);
}

}  // namespace tests
}  // namespace client
}  // namespace failless

#endif  // FAILLESS_TESTS_TESTS_CLI_SERIALIZATION_TESTS_H_
