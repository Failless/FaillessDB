#include "llsscli/core/client.h"
#include "llsscli/config/config.h"

using namespace failless::client::core;
using namespace failless::client::config;

int main(int argc, char **argv) {
    ClientConfig test_interface_data("user", "SEND my.txt FROM .", 0, "key", "127.0.0.1", "11564");
    Client user = Client(test_interface_data);
    user.Run();
}
