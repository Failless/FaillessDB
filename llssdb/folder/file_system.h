#ifndef FAILLESS_FILE_SYSTEM_H
#define FAILLESS_FILE_SYSTEM_H

#include <rocksdb/db.h>
#include <rocksdb/options.h>
#include <boost/noncopyable.hpp>
#include <string>

namespace failless::db::folder {

using std::string;

class FileSystemInterface : boost::noncopyable {
 public:
    FileSystemInterface() = default;
    virtual ~FileSystemInterface() = default;

    virtual bool Get(const string &key) = 0;
    virtual bool Set(const string &key, const int8_t *value) = 0;
    virtual bool GetRange(const string &key) = 0;
    virtual bool Remove(const string &key) = 0;
    //  virtual Serialize(u_int8_t)
};

class FileSystem : public FileSystemInterface {
 public:
    explicit FileSystem(const string &db_path);
    ~FileSystem() override;

    bool Get(const string &key) override;
    bool Set(const string &key, const int8_t *value) override;
    bool GetRange(const string &key) override;
    bool Remove(const string &key) override;

    rocksdb::DB *db_;
};

}  // namespace failless::db::folder

#endif  // FAILLESS_FILE_SYSTEM_H
