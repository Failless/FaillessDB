#include "llsscli/core/client.h"

using namespace failless::client::core;

int main(int argc, char **argv) {
    ClientConfig test_interface_data;
    test_interface_data.user_name = "user";
    test_interface_data.user_request = "SEND my.txt";

    test_interface_data.payload_dest_id = 0;
    test_interface_data.payload_key = "key";

    test_interface_data.db_host = "127.0.0.1";
    test_interface_data.db_port = "11564";
    Client user = Client(test_interface_data);
}
