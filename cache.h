#include <iterator>
#include <list>
#include <unordered_map>
#include <iostream>
#pragma once

namespace cache {
template<typename T, typename KeyT = int>
class cache_t 
{
    private:
        std::size_t size; //size of cache for imulation of finitness of memory
        struct T_and_counter //for keep counter with 
        {
            T data;
            int counter;
            KeyT key; //for erase element from hash_map
            bool operator==(const T_and_counter& right) const {return (data == right.data);} //for method find() in std::unordered_map
            bool operator!=(const T_and_counter& right) const {return (data != right.data);} //for logic
        };
        std::list<T_and_counter> cache; //list like a good container for this objective

        using ListIt = typename std::list<T_and_counter>::iterator;  //I will keep only iterators on list in hash_map
        std::unordered_map<KeyT, ListIt> hash_map;                   //for fast find elements in cache
    public:
        cache_t(std::size_t sz): size{sz} {}; //constructor by fix size

        bool full() const {return (size == cache.size());} //for imulation of finitness of memeory

        template<typename F> //function that return T and get KeyT
        bool lookup_update(KeyT key, F slow_get_page)
        {
            auto hit = hash_map.find(key);
            if (hit == hash_map.end()) //if not found key
            {
                if (full()) //if full
                {
                    //delete last
                    hash_map.erase(cache.back().key);
                    cache.pop_back();
                }
                //push from back
                //go to big-big memory for T type elem, do elem of list with T data
                //counter equal to 1 and key of request
                cache.push_back({slow_get_page(key), 1, key});
                //push iterator on last elem in hash_map by key 
                hash_map[key] = std::prev(cache.end());

                return false;
            }
            else //if found
            {
                hit->second->counter++; //incriment counter
                if (hit->second != cache.begin()) //if found no first element
                    if (hit->second->counter > std::prev(hit->second)->counter) //if need to swap
                        //swap in list
                        cache.splice(std::prev(hit->second), cache, hit->second);
                        /*I dont need to swap elements in hash_map because I keep
                        iterator kn elements in list*/
                return true;
            }
        }

        void dump()
        {
            std::cerr << "Size:"<<size << std::endl;
            int i = 0;
            for (auto x: cache)
                std::cerr << i++ << ":\nkey: " << x.key << "\ncounter: " << x.counter << '\n';
        }
};

}//namespace end
