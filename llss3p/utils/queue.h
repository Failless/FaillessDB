#ifndef FAILLESS_LLSSDB_COMMON_QUEUE_H_
#define FAILLESS_LLSSDB_COMMON_QUEUE_H_

#include <boost/core/noncopyable.hpp>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

namespace failless {
namespace common {
namespace utils {

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

template<class T>
void Queue<T>::Push(const T &item) {
    std::unique_lock <std::mutex> lock(mutex_add_);
    queue_.push(item);
    is_empty_.notify_one();
}

template<class T>
T Queue<T>::Pop() {
    std::unique_lock <std::mutex> lock(mutex_get_);
    is_empty_.wait(lock, [this]() { return !this->queue_.empty(); });
    T item = queue_.front();
    queue_.pop();
    return std::move(item);
}

template<class T>
bool Queue<T>::IsEmpty() const {
    return queue_.empty();
}

template<class T>
Queue<T>::Queue(size_t size) : queue_(size) {}

}  // namespace utils
}  // namespace db
}  // namespace failless

#endif  // FAILLESS_LLSSDB_COMMON_QUEUE_H_
