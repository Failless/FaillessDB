#ifndef LLSSDB_NETWORK_RESPONSE_H_
#define LLSSDB_NETWORK_RESPONSE_H_

#include <msgpack.hpp>
#include <boost/asio.hpp>

class Response {
  explicit Response(uint8_t* data);
  Response(Request& request);
};

#endif // LLSSDB_NETWORK_RESPONSE_H_
