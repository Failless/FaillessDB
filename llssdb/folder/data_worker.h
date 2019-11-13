#ifndef LLSSDB_FOLDER_DATA_WORKER_H_
#define LLSSDB_FOLDER_DATA_WORKER_H_

#include <boost/container/set.hpp>
#include <string>

// TODO(EgorBedov): consider making interface and then template

using std::string;


class IDataWorker {
public:
    IDataWorker() = default;
    ~IDataWorker() = default;
    virtual int Create() = 0;
    virtual int Read() = 0;
    virtual int Modify() = 0;
    virtual int Remove() = 0;
};


class DataWorker : public IDataWorker {
public:
    DataWorker() = default;
    explicit DataWorker(string directory);
    ~DataWorker() = default;

    int Create() override;
    int Read() override;
    int Modify() override;
    int Remove() override;
private:
    string directory_{};
    int length_ = 0;
    boost::container::set<int, string> dataset_{};
};

#endif // LLSSDB_FOLDER_DATA_WORKER_H_
