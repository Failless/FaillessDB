#include "cache.h"

#include <list>
#include <string>
#include <vector>
#include <unordered_map>


namespace failless {
namespace db {
namespace utils {

typedef std::string key_type;
typedef std::vector<uint8_t> data_type;

bool cache::find(const key_type& key, data_type *item) const {
    auto it = cache_.find(key);

    if (it == cache_.end())
        return false;
    else {
        if (item)
            *item = it->second.data;

        return true;
    }
}

void cache::insert(const key_type& key, const data_type &data) {
    if (max_size_ != 0U) {
        lru_.push_front(key);
        cur_size_ += key.size();
        lru_record_t rec(data, lru_.begin());

        std::pair<typename cache_t::iterator, bool> result =
                cache_.insert(typename cache_t::value_type(key, rec));

        if (result.second) {
            if (cur_size_ > max_size_) {
                erase(lru_.back());
            }
            cur_size_ += key.size() + rec.size;
        } else {    // pair existed
            auto &oldit = result.first;
            lru_.erase(oldit->second.lru_it);
            cur_size_ -= key.size() + (oldit->second.size - rec.size);
            oldit->second = rec;
        }
    }
}

void cache::erase(const key_type& key) {
    auto it = cache_.find(key);
    if (it != cache_.end()) {
        cur_size_ -= key.size() * 2 + it->second.size;
        cache_.erase(it);
        lru_.erase(it->second.lru_it);
    }
}

size_t cache::size() const {
    return cache_.size();
}

bool cache::empty() const {
    return (size() == 0);
}

void cache::clear() {
    cur_size_ = 0;
    cache_.clear();
    lru_.clear();
}

}
}
}