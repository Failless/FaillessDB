#include <iostream>
#include "llssdb/network/tcp_server.h"


int main(int argc, char **argv) {
    std::unique_ptr<failless::db::network::ITcpServer> tcp_server =
        std::make_unique<failless::db::network::TcpServer>("127.0.0.1", 11556);
    tcp_server->Listen();
    return 0;
}