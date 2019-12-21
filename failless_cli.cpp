#include "llsscli/config/config.h"
#include "llsscli/core/client.h"

int main(int argc, char **argv) {
    std::unique_ptr<failless::client::core::ClientInterface> client(
        new failless::client::core::Client());
    client->ReadNetSettings();
    client->ReadInput();
}