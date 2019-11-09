#ifndef LLSSCLIENT_CORE_CLIENT_H_
#define LLSSCLIENT_CORE_CLIENT_H_

#include <cstddef>
#include <string>
#include <cstddef>
#include <cstdint>
#include "llssclient/network/mp_client.h"
#include "llssclient/filesystem/filesystem.h"

namespace failless::client::client_core {

    using std::string;

    using namespace failless::client::mp_client_network;
    using namespace failless::client::filesystem_filesystem;

    class client_interface {
    public:
        explicit client_interface();
        virtual ~client_interface();
        virtual size_t read_query( string query ) = 0;
        virtual size_t send() = 0;
        virtual size_t params( int argc, char **argv ) = 0;
    private:
        virtual size_t parse_query( string query ) = 0;
    };

    class client : public client_interface {
    public:
        explicit client( uintptr_t interpreter, mp_client* mp_client, filesystem* filesystem, char* config );
        ~client();
        size_t read_query( string query );
        size_t send();
        size_t params( int argc, char **argv );
    private:
        size_t parse_query( string query );
    private:
        uintptr_t interpreter;
        mp_client* mp_client;
        filesystem* filesystem;
        char* config;
    };

}
#endif // LLSSCLIENT_CORE_CLIENT_H_
