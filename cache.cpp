#include "cache.h"
#include <vector>
#include <iostream>

int slow_get_page (int key) {return key;}

int main()
{
    cache::cache_t<int> my_cache (4);
    std::vector<int> data = {1, 1, 2, 2, 1, 2, 2, 3, 5, 7, 7, 7, 1, 2, 1, 1, 5, 5, 5, 5};
    
    for (auto x: data)
        my_cache.lookup_update(x, slow_get_page);
    my_cache.dump();

    return 0;
}