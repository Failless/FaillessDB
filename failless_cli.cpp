#include "llsscli/config/config.h"
#include "llsscli/core/client.h"

int main(int argc, char **argv) {
    // TODO [BUG] If payload = 0 => somewhere in stringstrema like \0 => stop of sending => lost
    // data
//    failless::client::config::ClientConfig test_interface_set("user", "SET key", 1, "key_value",
//                                                              "127.0.0.1", "11564", 10);
//    failless::client::config::ClientConfig test_interface_get("user", "GET key", 1, "key_value_get",
//                                                              "127.0.0.1", "11564", 10);
//    failless::client::config::ClientConfig test_interface_create("user", "CREATE folder_id", 1, "",
//                                                                 "127.0.0.1", "11564", 10);
//    failless::client::config::ClientConfig test_interface_register("user", "REGISTER login pass", 1,
//                                                                   "", "127.0.0.1", "11564", 10);
    std::unique_ptr<failless::client::core::ClientInterface> client(
        new failless::client::core::Client());
    client->ReadNetSettings();
    client->ReadInput();
//    client->Run();
}

// SEND my.txt FROM . =>
// SET key 'value'
// GET key
// CREATE (folder_id - optional else will be returned)
// REGISTER login pass