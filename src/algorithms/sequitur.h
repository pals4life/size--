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
    using Digram = std::pair<uint32_t, uint32_t>;

    struct Rule
    {
        uint32_t head;
        Digram body;

        Rule(uint32_t head, const Digram &body): head(head), body(body) {};
    };


    void decode(const std::unordered_map<Digram, uint32_t, boost::hash<Digram>> &rules, u_int32_t c, u_int32_t begin);

    std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(std::vector<unsigned char> string);
    // std::pair<std::vector<Variable>, std::vector<Production>>
}
