#include "llsscli/core/client.h"
#include "llsscli/config/config.h"

int main(int argc, char **argv) {
    failless::client::config::ClientConfig test_interface_data("user", "SEND my.txt FROM .", 0, "key", "127.0.0.1", "11564");
    failless::client::core::Client user = failless::client::core::Client(test_interface_data);
    user.Run();
}
