#ifndef FAILLESS_TESTS_TESTS_CLI_TEST_SERIALIZER_MOCK_H_
#define FAILLESS_TESTS_TESTS_CLI_TEST_SERIALIZER_MOCK_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <llss3p/utils/packet.h>
#include "llss3p/serialization/serializer.h"
#include "llsscli/config/config.h"

namespace failless {
namespace client {
namespace test {

namespace srz = common::serializer;

class MockSerializer : public srz::Serializer<common::utils::Packet> {
 public:
    MockSerializer() : Serializer<common::utils::Packet>(){};
    MOCK_METHOD1(Serialize, size_t(std::shared_ptr<common::utils::Packet>& current_task));
    MOCK_METHOD1(Deserialize, size_t(std::shared_ptr<std::stringstream>& current_task));
};

TEST(Serializer, Ser_And_Deser) {
    MockSerializer ser;

    std::shared_ptr<common::utils::Packet> current_task_in(new common::utils::Packet());
    std::shared_ptr<std::stringstream> current_task_out(new std::stringstream());

    EXPECT_CALL(ser, Serialize(current_task_in)).Times(1);
    EXPECT_EQ(ser.Serialize(current_task_in), 0);

    EXPECT_CALL(ser, Deserialize(current_task_out)).Times(1);
    EXPECT_EQ(ser.Deserialize(current_task_out), 0);
}

}  // namespace test
}  // namespace client
}  // namespace failless

#endif  // FAILLESS_TESTS_TESTS_CLI_TEST_SERIALIZER_MOCK_H_
