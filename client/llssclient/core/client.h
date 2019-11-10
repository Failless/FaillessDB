#ifndef LLSSCLIENT_CORE_CLIENT_H_
#define LLSSCLIENT_CORE_CLIENT_H_

#include <cstddef>
#include <string>
#include <cstddef>
#include <cstdint>
#include "llssclient/network/mp_client.h"
#include "llssclient/filesystem/filesystem.h"

namespace failless::client::client {

using std::string;

using namespace failless::client::mp_client;
using namespace failless::client::filesystem;

class ClientInterface {
public:
    ClientInterface() = default;
    ~ClientInterface() = default;
    virtual size_t ReadQuery( string query ) = 0;
    virtual size_t Send() = 0;
    virtual size_t Params( int argc, char **argv ) = 0;
    int Test(string query) {
        ReadQuery(query);
        return EXIT_SUCCESS;
    }
private:
    virtual size_t ParseQuery_( string query ) = 0;
};

class Client : public ClientInterface {
public:
    Client() = default;
    Client(MpClient* mp_client, FileSystem* filesystem, string config);
    ~Client() = default;
    size_t ReadQuery( string query ) override;
    size_t Send() override;
    size_t Params( int argc, char **argv ) override;
private:
    size_t ParseQuery_( string query ) override;
private:
    MpClient* mp_client_;
    FileSystem* filesystem_;
    string config_;
};

}
#endif // LLSSCLIENT_CORE_CLIENT_H_
