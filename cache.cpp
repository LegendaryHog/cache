#include "cache.h"
#include <vector>
#include <iostream>

int slow_get_page(int key) {return key;}

int main()
{
    std::size_t c_sz = 0;
    std::size_t d_sz = 0;

    std::cin >> c_sz >> d_sz;

    cache::cache_t<int> my_cache (c_sz);
    std::vector<int> data (d_sz);

    for (auto x: data)
        std::cin >> x;

    std::size_t hits = 0;
    for (auto x: data)
        hits += my_cache.lookup_update(x, slow_get_page);

    std::cout << hits;
    return 0;
}