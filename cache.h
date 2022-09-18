#pragma once
#include <iterator>
#include <list>
#include <unordered_map>
#include <iostream>

namespace cache {

template<typename T, typename KeyT = int>
class cache_t 
{
    private:
        std::size_t capacity; //capacity of cache for imulation of finitness of memory
        std::size_t size;     //quantity of elements in cache right now

        struct Cntr_n_list; //declaration of elem of cache list
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

        void pop_back()
        {
            //delete from hash_map last elem from last sublist
            hash_map.erase(cache.back().sublist.back().key);
            //delete from last sublist last elem
            cache.back().sublist.pop_back();

            //if after this last sublist became empty - delete last node of cache
            if (cache.back().sublist.size() == 0)               
                cache.pop_back();
            size--;
        }

        void push_back(T page, KeyT key)
        {
            //put elem in last sublist in back
            cache.back().sublist.push_back({page, key, std::prev(cache.end())});
            //put itr on elem in hash_map
            hash_map[key] = std::prev(cache.back().sublist.end());
            size++;
        }

        void push_front(T page, KeyT key)
        {
            //put elem in last sublist in back
            cache.back().sublist.push_front({page, key, std::prev(cache.end())});
            //put itr on elem in hash_map
            hash_map[key] = cache.back().sublist.begin();
            size++;
        }

        void replace (SubListIt itr)
        {
            //to simplify records
            auto& cntr   = itr->itr_up->counter;
            auto& itr_up = itr->itr_up;

            //if found elem with biggest counter
            //(in begin of cache)- create node with
            //counter ub by 1
            if (itr_up == cache.begin())
                cache.push_front({{}, cntr + 1});

            //if found elem not from begin - check existance of node
            //with counter ub by 1 - create if its dont exist
            else if (std::prev(itr_up)->counter != cntr + 1)
                cache.insert(itr_up, {{}, cntr + 1});

            //for simplify records
            auto prev_itr_up = std::prev(itr_up);

            //replace in back of sublist with counter up by 1 found node (its reconnect the pointers,no copy)
            prev_itr_up->sublist.splice(prev_itr_up->sublist.begin(), itr_up->sublist, itr);

            //if after replace sublist in node
            //in cache with counter of hit became
            //empty - delete
            if (itr_up->sublist.size() == 0)
                cache.erase(itr_up);

            //connect iterator on node of
            //cache in found elem on new node
            itr_up = prev_itr_up;
        }

    public:
        cache_t(std::size_t cap): capacity {cap}, size {0} {}; //constructor by fix size
        cache_t() {};
        
        std::size_t cap() const {return capacity;}
        bool full() const {return (size == capacity);} //for imulation of finitness of memeory
        bool empty() const {return (size == 0);}       //for check is cache empty

        template<typename F> //F - function that return T and get KeyT
        bool lookup_update(KeyT key, F slow_get_page)
        {
            auto hit = hash_map.find(key);
            if (hit == hash_map.end()) //if not found key
            {
                if (full()) //if full
                    //pop element from
                    //back from back sublist
                    pop_back();

                //make node with counter 1 in
                //cache if cache is empty
                if (empty())
                    cache.push_back({{}, 1});

                //make node with counter 1 in
                //cache if its not exist
                if (cache.back().counter != 1)
                    cache.push_back({{}, 1});

                //push elem in sublist with counter 1 in back
                push_front(slow_get_page(key), key);
            
                return false;
            }
            //if found
            replace (hit->second);
            return true;
        }

        void dump() const
        {
            for(auto x: cache)
                std::cout << x.counter <<":" << x.sublist.size() << std::endl;
        }
};

}//namespace end
