#ifndef FAILLESS_LLSSDB_COMMON_SETTINGS_H_
#define FAILLESS_LLSSDB_COMMON_SETTINGS_H_

#include <string>


namespace failless {
    namespace db {
        namespace common {

            struct Settings {
                std::string bind;
                std::string data_path;
                std::string logfile;
                std::string email;
                bool using_email;
                int threads;
                int tcp_keepalive;
                int save;
                long max_bytes;
                long query_max;
                bool setonly;
                bool readonly;
                bool lua;
                int users_to_table;
                int port;


                Settings()
                        : bind("localhost"),
                          data_path("/data"),
                          logfile("log.txt"),
                          email(""),
                          using_email(false),
                          threads(8),
                          tcp_keepalive(0),
                          save(0),
                          max_bytes(0),
                          query_max(0),
                          setonly(false),
                          readonly(false),
                          lua(false),
                          users_to_table(1),
                          port(8888) {}
            };

        }  // namespace common
    }  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_COMMON_SETTINGS_H_
