#include <iostream>
#include "llssdb/folder/data_worker.h"
#include "llssdb/network/tcp_server.h"

int main(int argc, char **argv) {
    std::shared_ptr<IDataWorker> data_worker(new DataWorker());
    data_worker->Create();
    std::unique_ptr<failless::db::network::ITcpServer> tcp_server =
        std::make_unique<failless::db::network::TcpServer>("127.0.0.1", 11556);
    tcp_server->Listen();
    return 0;
}