#include "../../cache.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>

const std::size_t MAX_CACHE_SIZE = 250000;
const std::size_t DELTA_CACHE_SIZE = 50000;
const int NUM_TEST = 5;

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

    for (auto i = 0; i < argc - 1; i++)
        for (auto j = 0; j < NUM_TEST; j++)
        {
            cache::cache_t<int> my_cache (MAX_CACHE_SIZE - j * DELTA_CACHE_SIZE);
            auto start = std::chrono::high_resolution_clock::now();
            test(vec_t_inf[i], my_cache);
            auto end = std::chrono::high_resolution_clock::now();
            long long work_tm_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << i << ": num of keys:     " << vec_t_inf[i].data.size() << std::endl <<"cache cpacity:      " << my_cache.cap() << std::endl << "time of work in ms: " << work_tm_in_ms << std::endl << std::endl;
        }
    return 0;
}

void test(test_inf& t_inf, cache::cache_t<int>& my_cache)
{
    for (auto x: t_inf.data)
        my_cache.lookup_update(x, slow_get_page);
}