//============================================================================
// @name        : repair.h
// @author      : Thomas Dooms
// @date        : 11/19/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <tuple>
#include <vector>
#include <unordered_map>
#include <queue>

#include <boost/sort/spreadsort/integer_sort.hpp>
#include <boost/heap/fibonacci_heap.hpp>

#include "../util/settings.h"
#include "../util/variable.h"
#include "../util/production.h"
#include "../util/timer.h"

namespace algorithm::repair
{

std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(const std::vector<uint8_t>& bytes, bool prepass = true)
{
    constexpr auto compose   = [](uint64_t lhs, uint64_t rhs) -> uint64_t { return (lhs << 32u) + rhs; };

    constexpr size_t prepass_cutoff = 3;  // magic number
    constexpr size_t byte_pairs = 65536;  // more magic numbers
    constexpr size_t cutoff = 100;        // all these magic numbers
    constexpr size_t min_count = 50;      // hmm

    Settings settings(Settings::reserved | Settings::smart_productions);

    // max 4*n space
    // vector of productions bodies
    std::vector<Production> productions;

    // will become the resulting string
    std::vector<Variable> string; // smart reserve

    // we use this scope so our vectors do not persist through the rest of the function
    // in this pass we quickly go through the bytes and pair the most frequent pairs in an efficient way
    // we make no "new" productions because we reserve the variables.
    Timer::start();
    if(prepass)
    {
        // define the number of pairs that must occur before we change them
        std::array<uint32_t, byte_pairs> frequencies{}; // more magic numbers

        for(size_t i = 0; i < bytes.size() - 1; i++)
        {
            const auto pair = Settings::convert_to_reserved(bytes[i], bytes[i+1]);
            frequencies[pair]++;
        }
        for(size_t i = 0; i < bytes.size() - 1; i++)
        {
            const auto pair = Settings::convert_to_reserved(bytes[i], bytes[i+1]);
            if(frequencies[pair] > prepass_cutoff)
            {
                string.emplace_back(pair);
                i++;
            }
            else
            {
                string.emplace_back(bytes[i]);
            }
        }
    }
    Timer::end();

    Timer::start();

    size_t count = std::numeric_limits<size_t>::max();
    size_t index = 0;
    size_t replaced = 0;

    std::unordered_map<uint64_t, std::pair<uint32_t, uint32_t>> map(string.size() / 2);

    while(count > min_count)
    {
        count = 0;
        replaced = 0;

        for(size_t i = 0; i < string.size() - 1; i++)
        {
            const auto [iter, emplaced] = map.try_emplace(compose(string[i], string[i + 1]), 1, 0);

            if(emplaced) continue;
            iter->second.first++;

            if(iter->second.first > cutoff)
            {
                if(Settings::is_reserved_rule(string[i], string[i + 1]))
                {
                    iter->second.second = Settings::convert_to_reserved(string[i], string[i + 1]);
                }
                else if(iter->second.second == 0)
                {
                    iter->second.second = settings.offset(index);
                    productions.emplace_back(std::array{string[i], string[i+1]});
                    if(string[i+1] == Settings::end())
                        std::cout << "a\n";
                    index++;
                }
                count++;
            }
        }
        for(size_t i = 0; i < string.size() - 1; i++)
        {
            const auto val  = compose(string[i], string[i + 1]);
            const auto iter = map.find(val);
            if(iter != map.end() and iter->second.first > cutoff)
            {
                string[i] = iter->second.second;
                string[i + 1] = Settings::end();
                i++;
                replaced++;
            }
        }

        // remove all the tombstones in the string
        const auto end = std::remove_copy_if(string.begin(), string.end(), string.begin(),
            [](const auto elem){ return elem == Settings::end(); });

        // resize to the end
        string.resize(end - string.begin());
        std::cout << "guessed  " << count    << " variables\n";
        std::cout << "replaced " << replaced << " variables\n\n";
    }

    Timer::end();

    std::cout << "used " << productions.size() << " productions\n";

    return std::make_tuple(settings, std::move(string), std::move(productions));
}


}


