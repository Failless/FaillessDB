#ifndef FAILLESS_LLSSDB_COMMON_QUEUE_H_
#define FAILLESS_LLSSDB_COMMON_QUEUE_H_

#include <boost/core/noncopyable.hpp>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

namespace failless {
namespace db {
namespace common {

template <class T>
class Queue : boost::noncopyable {
 public:
    Queue() = default;
    explicit Queue(size_t size);
    ~Queue() = default;

    void Push(const T &item);
    T Pop();
    [[nodiscard]] bool IsEmpty() const;

 private:
    std::mutex mutex_add_;
    std::mutex mutex_get_;
    std::condition_variable is_empty_;
    std::queue<T> queue_;
};

}  // namespace common
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_COMMON_QUEUE_H_
