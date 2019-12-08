#ifndef FAILLESS_TEST_SERIALIZER_UNIT_H
#define FAILLESS_TEST_SERIALIZER_UNIT_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>
#include "llss3p/serialization/serializer.h"
#include "llss3p/utils/data.h"
#include "llss3p/utils/packet.h"
#include "llsscli/config/config.h"

namespace failless {
namespace client {
namespace test {

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
    char* bytes_{};
    size_t size_{};
};

TEST_F(SeralizerImpl, SeralizeDeserialize) {
    EXPECT_EQ(serializer_->Serialize(packet_), 0);
    auto stream = serializer_->GetOutStringStream();
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

TEST(Serializer, Ser_And_Get) {
    std::unique_ptr<common::serializer::SerializerInterface<config::Task>> ser(
        new common::serializer::Serializer<config::Task>);

    std::unique_ptr<config::Task> current_task_in(new config::Task());
    std::unique_ptr<std::stringstream> current_task_out(new std::stringstream());
    std::unique_ptr<std::vector<unsigned char>> value(
        new std::vector<unsigned char>{'a', 'b', 'c', 'd'});

    std::unique_ptr<std::string> key(new std::string{"abcd_key"});
    std::unique_ptr<std::string> client_id(new std::string{"abcd_client"});
    std::unique_ptr<std::string> query(new std::string{"abcd_query"});

    std::unique_ptr<config::Data> current_data(new config::Data(value, 6, 0, key));
    std::shared_ptr<config::Task> current_task(new config::Task(client_id, query, current_data));

    ser->Serialize(*current_task);

    std::shared_ptr<std::stringstream> out_buf_(ser->GetOutStringStream());
    ser->Deserialize(const_cast<char*>(out_buf_->str().c_str()), out_buf_->str().size());

    std::shared_ptr<config::Task> current_task_check(ser->GetInConfig());

    EXPECT_EQ(*current_task.get()->payload.get()->value.get(),
              *current_task_check.get()->payload.get()->value.get());
}

}  // namespace test
}  // namespace client
}  // namespace failless

#endif  // FAILLESS_TEST_SERIALIZER_UNIT_H
