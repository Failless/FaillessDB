#ifndef FAILLESS_TEST_SERIALIZER_UNIT_H
#define FAILLESS_TEST_SERIALIZER_UNIT_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "llsscli/config/config.h"

namespace failless {
namespace client {
namespace test {

TEST(Serializer, Ser_And_Get) {
    Serializer ser;

    std::unique_ptr<config::Task> current_task_in(new config::Task());
    std::unique_ptr<std::stringstream> current_task_out(new std::stringstream());
    std::unique_ptr<std::vector<unsigned char> > value(
        new std::vector<unsigned char>{'a', 'b', 'c', 'd'});

    std::unique_ptr<std::string> key(new std::string{"abcd_key"});
    std::unique_ptr<std::string> client_id(new std::string{"abcd_client"});
    std::unique_ptr<std::string> query(new std::string{"abcd_query"});

    std::unique_ptr<config::Data> current_data(new config::Data(value, 6, 0, key));
    std::shared_ptr<config::Task> current_task(new config::Task(client_id, query, current_data));

    ser.Serialize(current_task);

    std::shared_ptr<std::stringstream> out_buf_(ser.GetOutStringStream());

    ser.Deserialize(out_buf_);

    std::shared_ptr<config::Task> current_task_check(ser.GetInConfig());

    EXPECT_EQ(*current_task.get()->payload.get()->value.get(),
              *current_task_check.get()->payload.get()->value.get());
}

}  // namespace test
}  // namespace client
}  // namespace failless

#endif  // FAILLESS_TEST_SERIALIZER_UNIT_H
