// =====================================================================
// @name: sequitur.h
// @project: SIZE--
// @author: Mano Marichal
// @date: 18.11.19
// @copyright: BA2 Informatica - Mano Marichal - University of Antwerp
// @description: 
// =====================================================================

#pragma once

#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <fstream>
#include <unordered_map>
#include <boost/functional/hash.hpp>

#include "../util/variable.h"
#include "../util/production.h"
#include "../util/settings.h"
#include <tuple>

namespace algorithm::sequitur
{
    struct TableValue
    {
        std::vector<uint32_t>::iterator location;
        uint32_t leftSymbol;
        uint32_t rightSymbol;
        TableValue(const std::vector<uint32_t> ::iterator &location, const uint32_t &left, const uint32_t &right): location(location), leftSymbol(left), rightSymbol(right) {};
        TableValue(const std::vector<uint32_t> ::iterator &location):location(location) {};
    };
    using Digram = std::pair<uint32_t, uint32_t>;

    void decode(const std::unordered_map<Digram, uint32_t, boost::hash<Digram>> &rules, u_int32_t c, u_int32_t begin);

    std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(std::vector<unsigned char> string);
    // std::pair<std::vector<Variable>, std::vector<Production>>
}
