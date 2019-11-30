#include "queue.h"

namespace failless {
namespace common {
namespace utils {

template <class T>
void Queue<T>::Push(const T &item) {
    std::unique_lock<std::mutex> lock(mutex_add_);
    queue_.push(item);
    is_empty_.notify_one();
}

template <class T>
T Queue<T>::Pop() {
    std::unique_lock<std::mutex> lock(mutex_get_);
    is_empty_.wait(lock, [this]() { return !this->queue_.empty(); });
    T item = queue_.front();
    queue_.pop();
    return std::move(item);
}

template <class T>
bool Queue<T>::IsEmpty() const {
    return queue_.empty();
}

template <class T>
Queue<T>::Queue(size_t size) : queue_(size) {}

}  // namespace utils
}  // namespace common
}  // namespace failless
