#include "../../cache.h"
#include <vector>
#include <iostream>
#include <fstream>

struct test_inf
{
    std::vector<int> data;
    std::size_t      cache_size;
    int              hits;
    test_inf(const char* file_name)
    {
        std::ifstream in;
        std::size_t data_size = 0;
        in.open(file_name);
        in >> data_size;
        data.resize(data_size);
        for(std::size_t i = 0; i < data_size; i++)
            in >> data[i];
        in >> cache_size >> hits;
        in.close();
    }   
    test_inf(): cache_size{0}, hits{0} {};
};

int slow_get_page (int key) {return key;}

int test (test_inf& t_inf, cache::cache_t<int>& my_cache);

int main(int argc, char* argv[])
{
    std::vector<test_inf> vec_t_inf (argc - 1);
    for (auto i = 1; i < argc; i++)
    {
        test_inf new_inf (argv[i]);
        vec_t_inf[i - 1] = std::move(new_inf);
    }

    std::vector<cache::cache_t<int>> vec_my_cache (argc - 1);
    for (auto i = 0; i < argc - 1; i++)
    {
        cache::cache_t<int> new_cache (vec_t_inf[i].cache_size);
        vec_my_cache[i] = std::move(new_cache);
    }

    for (auto i = 0; i < argc - 1; i++)
    {
        int hits = test(vec_t_inf[i], vec_my_cache[i]);

        if (hits == vec_t_inf[i].hits)
            std::cout << i << ":Succes\n";
        else
            std::cout << i<< ":Error: hits = " << hits << " but hits from test = " << vec_t_inf[i].hits << '\n';
    }
    vec_my_cache[0].dump();
    return 0;
}

int test(test_inf& t_inf, cache::cache_t<int>& my_cache)
{
    int hits = 0;
    for (auto x: t_inf.data)
        hits += my_cache.lookup_update(x, slow_get_page);
    return hits;
}