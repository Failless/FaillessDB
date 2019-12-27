#ifndef FAILLESS_LLSSDB_UTILS_SETTINGS_H_
#define FAILLESS_LLSSDB_UTILS_SETTINGS_H_

#include <string>
#include <utility>

namespace failless {
namespace db {
namespace utils {

struct Settings {
    std::string bind;
    std::string data_path;
    std::string log_path;
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
    bool do_backup;
    int users_to_table;
    int port;

    Settings()
        : bind("localhost"),
          data_path("/data"),
          log_path("/tmp/failless/log"),
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
          do_backup(false),
          users_to_table(1),
          port(8888) {}
};

struct WorkerSettings {
    WorkerSettings() = default;
    explicit WorkerSettings(std::string _db_path, bool _do_backup = false)
      : db_path(std::move(_db_path)),
        do_backup(_do_backup) {}
    std::string db_path;
    bool do_backup = false;
};


}  // namespace utils
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_UTILS_SETTINGS_H_
