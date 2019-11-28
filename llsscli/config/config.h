#ifndef FAILLESS_CONFIG_CONFIG_H_
#define FAILLESS_CONFIG_CONFIG_H_

#include <string>
#include <msgpack.hpp>

namespace failless {
namespace client {
namespace config {

struct Data {
    // https://github.com/msgpack/msgpack-c/issues/243
    std::unique_ptr< std::vector<unsigned char> > value;
    size_t size = 0;
    size_t folder_id = 0;
    std::unique_ptr< std::string > key;

    // http://ludwig125.hatenablog.com/entry/2017/12/01/232952
    // http://ludwig125.hatenablog.com/entry/2017/11/22/234335
    MSGPACK_DEFINE_MAP(value, size, folder_id, key);

    Data() = default;
    Data(std::unique_ptr< std::vector<unsigned char> >& v, size_t s, size_t f_id, std::unique_ptr< std::string >& k) :
                                                                                value(std::move(v)),
                                                                                size(s),
                                                                                folder_id(f_id),
                                                                                key(std::move(k)) {}
};

struct Task {
    std::unique_ptr< std::string > client_id;
    std::unique_ptr< std::string > query;
    std::unique_ptr< Data > payload;

    MSGPACK_DEFINE_MAP(client_id, query, payload);

    Task() = default;
    Task(std::unique_ptr< std::string >& c_id, std::unique_ptr< std::string >& q, std::unique_ptr< Data >& p) :
                                                                                client_id(std::move(c_id)),
                                                                                query(std::move(q)),
                                                                                payload(std::move(p)) {}
};

struct ClientConfig {
    std::string user_name{};
    std::string user_request{};

    int payload_dest_id = 0;
    std::string payload_key{};

    std::string db_host{};
    std::string db_port{};

    ClientConfig(std::string user_n, std::string user_r, int payload_d, std::string payload_k, std::string db_h, std::string db_p) :
                                                                                user_name(user_n),
                                                                                user_request(user_r),
                                                                                payload_dest_id(payload_d),
                                                                                payload_key(payload_k),
                                                                                db_host(db_h),
                                                                                db_port(db_p) {}
};

struct NetworkConfig {
    std::string db_host{};
    std::string db_port{};

    NetworkConfig(std::string db_h, std::string db_p) : db_host(db_h), db_port(db_p) {}
};

} // namespace core
} // namespace llsscli
} // namespace failless

#endif //FAILLESS_CONFIG_CONFIG_H_
