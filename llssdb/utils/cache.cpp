#include "cache.h"

#include <list>
#include <string>
#include <vector>
#include <unordered_map>

#include <boost/log/trivial.hpp>


namespace failless {
namespace db {
namespace utils {

typedef std::string key_type;
typedef std::vector<uint8_t> data_type;

bool cache::get(const key_type& key, data_type& data) const {
    auto it = cache_.find(key);

    if (it == cache_.end())
        return false;
    else {
        data = it->second.data;
        BOOST_LOG_TRIVIAL(debug) << "[TW]: \"" << key << "\" retrieved from RAM";

        return true;
    }
}

void cache::insert(const key_type& key, const data_type &data) {
    if (max_size != 0U) {
        lru_.push_front(key);
        cur_size += key.size();
        lru_record_t rec(data, lru_.begin());

        std::pair<typename cache_t::iterator, bool> result =
                cache_.insert(typename cache_t::value_type(key, rec));

        if (result.second) {
            if (cur_size > max_size) {
                erase(lru_.back());
            }
            cur_size += key.size() + rec.size;
        } else {    // pair existed
            auto &oldit = result.first;
            lru_.erase(oldit->second.lru_it);
            cur_size -= key.size() + (oldit->second.size - rec.size);
            oldit->second = rec;
        }
        BOOST_LOG_TRIVIAL(debug) << "[TW]: \"" << key << "\" loaded from HDD into RAM";
    }
}

void cache::erase(const key_type& key) {
    auto it = cache_.find(key);
    if (it != cache_.end()) {
        cur_size -= key.size() * 2 + it->second.size;
        cache_.erase(it);
        lru_.erase(it->second.lru_it);
        BOOST_LOG_TRIVIAL(debug) << "[TW]: \"" << key << "\" erased from RAM";
    }
}

size_t cache::size() const {
    return cache_.size();
}

bool cache::empty() const {
    return (size() == 0);
}

void cache::clear() {
    cur_size = 0;
    cache_.clear();
    lru_.clear();
    BOOST_LOG_TRIVIAL(debug) << "[TW]: Unloaded everything from RAM";
}

}
}
}