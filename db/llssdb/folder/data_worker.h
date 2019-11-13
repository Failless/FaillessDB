#ifndef LLSSDB_FOLDER_DATA_WORKER_H_
#define LLSSDB_FOLDER_DATA_WORKER_H_

#include <set>
#include <string>


class IDataWorker {
public:
    explicit IDataWorker() :
        directory_(SetDirectory()),
        length_(SetLength())
//        dataset_(SetDataSet())
        {}
    virtual ~IDataWorker() = default;
    virtual bool Create() = 0;
    virtual bool Read() = 0;
    virtual bool Update() = 0;
    virtual bool Delete() = 0;

private:
    virtual std::string SetDirectory() {
        return "/dev/null"; // TODO: remove me
    }
    virtual int32_t SetLength() {
        return 0;
    }
//    virtual std::set<int, std::string> SetDataSet() {} // TODO: nah

    std::string directory_{};
    int length_ = 0;
    std::set<int, std::string> dataset_{};
};


class DataWorker : public IDataWorker {
public:
    ~DataWorker() override = default;

    bool Create() override;
    bool Read() override;
    bool Update() override;
    bool Delete() override;
};

#endif // LLSSDB_FOLDER_DATA_WORKER_H_
