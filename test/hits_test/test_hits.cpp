#include "../../cache.h"
#include <vector>
#include <iostream>
#include <fstream>

struct test_inf
{
    std::vector<int> data;
    std::size_t      cache_size;
    int              hits;
    test_inf(const char* file_name, const char* answ_file)
    {
        std::ifstream in_test;
        std::size_t data_size = 0;
        in_test.open(file_name);
        in_test >> cache_size >> data_size;
        data.resize(data_size);
        for(std::size_t i = 0; i < data_size; i++)
            in_test >> data[i];
        in_test.close();
        std::ifstream in_answ;
        in_answ.open(answ_file);
        in_answ >> hits;
        in_answ.close();
    }   
    test_inf(): cache_size{0}, hits{0} {};
};

int slow_get_page (int key) {return key;}

int test (test_inf& t_inf, cache::cache_t<int>& my_cache);

int main(int argc, char* argv[])
{
    std::vector<test_inf> vec_t_inf ((argc - 1)/2);
    for (auto i = 0; i < (argc - 1)/2; i++)
    {
        test_inf new_inf (argv[2*i + 1], argv[2*i + 2]);
        vec_t_inf[i] = std::move(new_inf);
    }

    std::vector<cache::cache_t<int>> vec_my_cache (argc - 1);
    for (auto i = 0; i < (argc - 1)/2; i++)
    {
        cache::cache_t<int> new_cache (vec_t_inf[i].cache_size);
        vec_my_cache[i] = std::move(new_cache);
    }

    for (auto i = 0; i < (argc - 1)/2; i++)
    {
        int hits = test(vec_t_inf[i], vec_my_cache[i]);

        if (hits == vec_t_inf[i].hits)
            std::cout << i << ":Succes\n";
        else
            std::cout << i<< ":Error: hits = " << hits << " but hits from test = " << vec_t_inf[i].hits << '\n';
    }
    return 0;
}

int test(test_inf& t_inf, cache::cache_t<int>& my_cache)
{
    int hits = 0;
    for (auto x: t_inf.data)
        hits += my_cache.lookup_update(x, slow_get_page);
    return hits;
}