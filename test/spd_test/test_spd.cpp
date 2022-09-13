#include "../../cache.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>

const std::size_t CACHE_SIZE = 250000;

struct test_inf
{
    std::vector<int> data;
    test_inf(const char* file_name)
    {
        std::ifstream in;
        std::size_t data_size = 0;
        in.open(file_name);
        in >> data_size;
        data.resize(data_size);
        for(std::size_t i = 0; i < data_size; i++)
            in >> data[i];
        in.close();
    }   
    test_inf() {};
};

int slow_get_page (int key) {return key;}

void test (test_inf& t_inf, cache::cache_t<int>& my_cache);

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
        cache::cache_t<int> new_cache (CACHE_SIZE);
        vec_my_cache[i] = std::move(new_cache);
    }

    for (auto i = 0; i < argc - 1; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        test(vec_t_inf[i], vec_my_cache[i]);
        auto end = std::chrono::high_resolution_clock::now();
        long long work_tm_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << i << ": num of keys:" << vec_t_inf[i].data.size() << std::endl << "time of work in ms: " << work_tm_in_ms << std::endl;
    }
    return 0;
}

void test(test_inf& t_inf, cache::cache_t<int>& my_cache)
{
    for (auto x: t_inf.data)
        my_cache.lookup_update(x, slow_get_page);
}