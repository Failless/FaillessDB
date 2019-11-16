#ifndef FAILLESS_FILE_SYSTEM_H
#define FAILLESS_FILE_SYSTEM_H


#include <boost/noncopyable.hpp>

class FileSystemInterface : boost::noncopyable {
public:
  explicit FileSystemInterface() = default;
  virtual ~FileSystemInterface() = default;

  virtual u_int8_t Get(int key) = 0;
  virtual bool Set(int key, u_int8_t data) = 0;
  virtual u_int8_t GetRange(int key) = 0;
  virtual bool Remove(int key) = 0;
//  virtual Serialize(u_int8_t)
};

class FileSystem : public FileSystemInterface {
public:
  ~FileSystem() override = default;

  u_int8_t Get(int key) override {
    // Call RocksDB
    return true;
  }

  bool Set(int key, u_int8_t data) override {
    // Call RocksDB
    return true;
  }

  u_int8_t GetRange(int key) override {
    // Call RocksDB
    return true;
  }

  bool Remove(int key) override {
    // Call RocksDB
    return true;
  }
};


#endif // FAILLESS_FILE_SYSTEM_H
