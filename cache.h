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
        std::size_t capacity; //capacity of cache for imulation of finitness of memory
        std::size_t size;     //quantity of elements in cache right now

        struct Cntr_n_list; //declaration of elem of main list
        struct T_n_key      
        {
            T data;
            KeyT key; //for erase element from hash_map
            using ListIt = typename std::list<Cntr_n_list>::iterator;
            ListIt itr_up;
        };

        struct Cntr_n_list
        {
            std::list<T_n_key> sublist;
            int counter = 1;
        };

        std::list<Cntr_n_list> cache; //list like a good container for this objective

        using SubListIt = typename std::list<T_n_key>::iterator;        //I will keep only iterators on list in hash_map
        std::unordered_map<KeyT, SubListIt> hash_map;                   //for fast find elements in cache
    public:
        cache_t(std::size_t cap): capacity {cap}, size {0} {}; //constructor by fix size
        cache_t(): size{0} {};                                 //default constructor

        bool full() const {return (size == capacity);} //for imulation of finitness of memeory
        bool empty() const {return (size == 0);}

        template<typename F> //F - function that return T and get KeyT
        bool lookup_update(KeyT key, F slow_get_page)
        {
            auto hit = hash_map.find(key);
            if (hit == hash_map.end()) //if not found key
            {
                if (full()) //if full
                {
                    //delete last
                    hash_map.erase(cache.back().sublist.back().key);
                    cache.back().sublist.pop_back();
                    if (cache.back().sublist.size() == 0)
                        cache.pop_back();
                    size--;
                }
                if (empty())
                    cache.push_back({{}, 1});
                if (cache.back().counter != 1)
                    cache.push_back({{}, 1});

                cache.back().sublist.push_back({slow_get_page(key), key, std::prev(cache.end())});
                hash_map[key] = std::prev(cache.back().sublist.end());
                size++;
            
                return false;
            }
            else //if found
            {
                auto  cntr_hit  = hit->second->itr_up->counter;
                auto& itr_hit   = hit->second;
                auto& itr_up_hit= hit->second->itr_up;

                if (itr_up_hit == cache.begin())
                    cache.push_front({{}, cntr_hit + 1});
                else if (std::prev(itr_up_hit)->counter != cntr_hit + 1)
                    cache.insert(itr_up_hit, {{}, cntr_hit + 1});

                auto prev_itr_up_hit = std::prev(itr_up_hit);
                prev_itr_up_hit->sublist.splice(prev_itr_up_hit->sublist.end(), itr_up_hit->sublist, itr_hit);

                if (itr_up_hit->sublist.size() == 0)
                    cache.erase(itr_up_hit);

                itr_up_hit = prev_itr_up_hit;
                return true;
            }
        }
};

}//namespace end
