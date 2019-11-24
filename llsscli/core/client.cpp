#include "llsscli/core/client.h"

namespace failless {
namespace client {
namespace core {

Client::Client() {
    try {
        cout << "Client is starting..." << endl;
        boost::asio::io_service IO_Service;

        tcp::resolver Resolver(IO_Service);

        tcp::resolver::query Query("127.0.0.1", "11564");

        tcp::resolver::iterator EndPointIterator = Resolver.resolve(Query);

        network_client_ = new NetworkClient(IO_Service, EndPointIterator);

        cout << "Client is started!" << endl;

        cout << "Enter a query string " << endl;

        std::thread ClientThread(boost::bind(&boost::asio::io_service::run, &
                IO_Service));

//        network_client_->Close();
        ClientThread.join();
    }
    catch (std::exception &e) {
        cerr << e.what() << endl;
    }

    cout << "\nClosing";
}

Client::~Client() {
    delete network_client_;
}

size_t Client::Run() {
    //открыть файл настроек
    //=прочитать user_name, host, port, home folder
    //запустить файловую систему (передать home folder)
    //запустить сериализатор
    //запустить сетевой клиент (передать host port)

    //вернуть статус запуска
    return 0;
}

size_t Client::SendRequestWithCB_(stringstream serialized_query, uintptr_t call_back) {
    return 0;
}

size_t Client::SerializeQuery_(string query) {
    return 0;
}

size_t Client::ExecQuery_(string tokens) {
    return 0;
}

size_t Client::ParseInput_(string raw_query) {
    return 0;
}

size_t Client::ReadInput_(int argc, char **argv) {
    return 0;
}

}  // namespace core
}  // namespace client
}  // namespace failless
