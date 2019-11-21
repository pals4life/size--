//============================================================================
// @name        : repair.cpp
// @author      : Thomas Dooms
// @date        : 11/19/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <boost/sort/spreadsort/integer_sort.hpp>
#include <unordered_map>
#include <queue>
#include "repair.h"

using Pair = uint16_t;

Pair to_pair(uint8_t a, uint8_t b) { return (a << 8u) + b; }

std::tuple<Settings, std::vector<Variable>, std::vector<Production>> algorithm::repair::compress(const std::vector<uint8_t>& bytes)
{
    Settings settings;
    // 4*n space
    // vector of variables, can contains tombstones
    std::vector<Variable> string(bytes.begin(), bytes.end());

    // max 6*n space
    // positions of pair occurrences, queue can index into this vector
    std::vector<std::pair<uint16_t, uint32_t>> positions(bytes.size());

    // max 20 * n ^ (1/3) space
    // (variable0, variable1, start index, interval length, frequency
    std::vector<std::tuple<Variable, Variable, uint32_t, uint32_t, uint32_t>> queue;

    // 256 * 256 * 4 -> O(1) space
    // frequencies of pairs
    std::vector<uint32_t> frequencies(Settings::byte_end * Settings::byte_end);

    // max 4*n space
    // vector of productions bodies
    std::vector<Production> productions;

    // we use this value to determine of a pair is "high frequency" or not
    const uint32_t cutoff = std::floor(std::pow(bytes.size(), 2.0 / 3.0));

    // first loop to count the pairs and to put them in the return string
    for(size_t i = 0; i < bytes.size() - 1; i++)
    {
        frequencies[to_pair(bytes[i], bytes[i+1])]++;
    }

    for(size_t i = 0, j = 0; i < bytes.size() - 1; i++)
    {
        const auto pair = to_pair(bytes[i], bytes[i+1]);
        if(frequencies[pair] > cutoff)
        {
            positions[j] = {pair, i};
        }
    }

    boost::sort::spreadsort::integer_sort(positions,
             [](const auto elem, const auto shift){ return elem.first >> shift;   },
            [](const auto lhs , const auto rhs)  { return lhs.first < rhs.first; });

    return std::make_tuple(settings, std::move(string), std::move(productions));
}