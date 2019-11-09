#ifndef LLSSCLIENT_FILESYSTEM_FILESYSTEM_H_
#define LLSSCLIENT_FILESYSTEM_FILESYSTEM_H_

#include <cstddef>
#include <cstdint>

namespace failless::client::filesystem_filesystem {

    class filesystem_interface {
    public:
        explicit filesystem_interface();
        virtual ~filesystem_interface();
        virtual size_t read_file( uintptr_t data ) = 0;
        virtual size_t write_file( uintptr_t data ) = 0;
    private:
    };

    class filesystem : public filesystem_interface {
    public:
        explicit filesystem( char file_name );
        ~filesystem();
        size_t read_file( uintptr_t data );
        size_t write_file( uintptr_t data );
    private:
        char file_name;
    };
}

#endif // LLSSCLIENT_FILESYSTEM_FILESYSTEM_H_
