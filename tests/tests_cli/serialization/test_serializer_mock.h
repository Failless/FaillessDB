#ifndef FAILLESS_TEST_SERIALIZER_MOCK_H
#define FAILLESS_TEST_SERIALIZER_MOCK_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sstream>
#include <string>
/*
#include "llsscli/config/config.h"
#include "llss3p/serialization/serializer.h"

namespace failless {
namespace client {
namespace test {

using common::serializer::Serializer;
using std::string;
using ::testing::_;
using ::testing::AtLeast;

class MockSerializer : public Serializer<config::Task> {
 public:
    MockSerializer() : Serializer<config::Task>(){};
    MOCK_METHOD1(Serialize, size_t(std::shared_ptr<config::Task>& current_task));
    MOCK_METHOD1(Deserialize, size_t(std::shared_ptr<std::stringstream>& current_task));
};

TEST(Serializer, Ser_And_Deser) {
    MockSerializer ser;

    std::shared_ptr<config::Task> current_task_in(new config::Task());
    std::shared_ptr<std::stringstream> current_task_out(new std::stringstream());

    EXPECT_CALL(ser, Serialize(current_task_in)).Times(1);
    EXPECT_EQ(ser.Serialize(current_task_in), 0);

    EXPECT_CALL(ser, Deserialize(current_task_out)).Times(1);
    EXPECT_EQ(ser.Deserialize(current_task_out), 0);
}
}  // namespace test
}  // namespace client
}  // namespace failless
 */

#endif  // FAILLESS_TEST_SERIALIZER_MOCK_H
