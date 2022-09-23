#include "../../cache.h"
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

struct JoJoT {
    bool stand = false;
    std::string stand_name = "";
    bool jostar_blood = false;
    bool hamon = false;
};

JoJoT slow_get_jojo (const std::string& char_name, const std::vector<std::pair<std::string, JoJoT>>& data)
{
    for(auto x: data)
        if (char_name == x.first)
            return x.second;
    return {};
}

int main()
{  
    std::vector<std::pair<std::string, JoJoT>> data = {
        {"Joseph Jostar",         {true, "Hermit Purple",   true,  true}},
        {"Jonathan Jostar",       {false, "", true,         true}},
        {"Dio Brando",            {true, "The World",       true,  false}},
        {"Erina Pendelton",       {}},
        {"Jotaro Kujo",           {true, "Star Platinum",   true,  false}},
        {"Muhammad Avdol",        {true, "Magician's Red"}},
        {"Jean Pierre Polnareff", {true, "Silver Chariot"}},
        {"Noriaki Kakyoin",       {true, "Hierophant Green", false, false}},
        {"Hol Horse",             {true, "Emperor"}},
        {"Josuke Higashikata",    {true, "Crazy Dimond",     true}}
    };

    cache::cache_t<JoJoT, std::string> my_cache (2);

    std::size_t hits = 0;
    for (auto x: data)
        hits += my_cache.lookup_update(x.first, [&](const std::string& char_name){return slow_get_jojo(char_name, data);});
    for (auto x: data)
        hits += my_cache.lookup_update(x.first, [&](const std::string& char_name){return slow_get_jojo(char_name, data);});

    std::cout << hits << std::endl;
    return 0;
}