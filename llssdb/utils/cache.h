#ifndef FAILLESS_LLSSDB_UTILS_CACHE_H
#define FAILLESS_LLSSDB_UTILS_CACHE_H

#include <list>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>


namespace failless {
namespace db {
namespace utils {

typedef std::string key_type;
typedef std::vector<uint8_t> data_type;

class cache {
    typedef std::list<key_type> lru_t;
    lru_t lru_;

    struct lru_record_t {
        data_type data;
        size_t size = 0;
        typename lru_t::iterator lru_it;

        lru_record_t(data_type dat, const typename lru_t::iterator &it)
                : data(std::move(dat)), lru_it(it) {
            size = data.size() + lru_it->size() + sizeof(size);
        }
    };

    typedef std::unordered_map <key_type, lru_record_t> cache_t;
    cache_t cache_;
public:
    cache(long bytes) : max_size(bytes) {}

    bool get(const key_type& key, data_type& item) const;

    void insert(const key_type& key, const data_type &data);

    void erase(const key_type& key);

    size_t size() const;

    bool empty() const;

    void clear();

    long max_size;
    long cur_size = 0;
};

}
}
}


#endif //FAILLESS_LLSSDB_UTILS_CACHE_H
