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

#include "../util/variable.h"
#include "../util/production.h"
#include "../util/settings.h"
#include <tuple>

namespace algorithm::sequitur
{
     struct Digram
     {
         uint32_t first;
         uint32_t second;
         Digram(uint32_t first, uint32_t second) : first(first), second(second) {}
         Digram()= default;
         Digram(const Digram &digram2)
         {
             first = digram2.first;
             second = digram2.second;
         }
         bool operator==(const Digram &rhs)
         {
             return (second == rhs.second and first == rhs.first);
         }

     };

    struct Rule
    {
        uint32_t head;
        Digram body;

        Rule(uint32_t head, const Digram &body): head(head), body(body) {};
    };


    void decode(std::vector<Rule> &rules, std::fstream &file, u_int32_t c, u_int32_t begin);

    std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(std::vector<unsigned char> string);
    // std::pair<std::vector<Variable>, std::vector<Production>>
}
