#include <iterator>
#include <list>
#include <unordered_map>
#pragma once

template<typename T, typename KeyT = int>
class cache_t 
{
    private:
        std::size_t size;
        struct T_and_counter
        {
            T data;
            int counter;
            bool operator==(const T_and_counter& right) const {return (data == right.data);}
            bool operator!=(const T_and_counter& right) const {return (data != right.data);}
            bool operator< (const T_and_counter& right) const {return (counter < right.counter);}
        };
        std::list<T_and_counter> cache;

        using ListIt = typename std::list<T_and_counter>::iterator;
        std::unordered_map<KeyT, ListIt> hash_map;
    public:
        cache_t(std::size_t sz): size{sz} {};
        bool full() const {return (size == cache.size());}

        template<typename F>
        bool lookup_update(KeyT key, F slow_get_page)
        {
            auto hit = hash_map.find(key);
            if (hit == hash_map.end()) //if not found key
            {
                if (cache.full())
                {
                    hash_map.erase(cache.back());
                    cache.pop_back();
                }
                cache.push_back({slow_get_page(key), 0});
                hash_map[key] = cache.back();
                cache.back().counter++;
                return false;
            }
            else
            {
                hit->counter++;
                if (hit != cache.begin())
                    if ((hit - 1)->counter < hit->counter)
                        std::swap(hit, hit - 1);
                return true;
            }
        }
};

