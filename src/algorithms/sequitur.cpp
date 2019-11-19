// =====================================================================
// @name: sequitur.cpp
// @project: SIZE--
// @author: Mano Marichal
// @date: 18.11.19
// @copyright: BA2 Informatica - Mano Marichal - University of Antwerp
// @description: 
// =====================================================================

#include "sequitur.h"
#include <iostream>
#include <fstream>

/*
void algorithm::sequitur::decode(const std::unordered_map<Digram, uint32_t, boost::hash<Digram>> &rules, u_int32_t c, u_int32_t begin)
{
    if (c < begin)
    {
        std::cout << char(c) << std::flush;
        return;
    }

    for (auto &rule:rules)
    {
        if (rule.second == c)
        {
            decode(rules,rule.first.first, begin);
            decode(rules,rule.first.second, begin);
            return;
        }
    }
}

 */

// TODO: nieuw gevormde digrams, rule utility, thomas adt muur afstemmen
std::tuple<Settings, std::vector<Variable>, std::vector<Production>> algorithm::sequitur::compress(
        std::vector<unsigned char> string)
{
    Settings settings(Settings::Flags::noflags);

    u_int32_t base = settings.begin();

    std::unordered_map<Digram, uint32_t, boost::hash<Digram>> ruleIndex;
    std::unordered_map<Digram, std::vector<uint32_t>::iterator, boost::hash<Digram>> digramIndex;

    std::vector<uint32_t> input(string.begin(), string.end());

    auto current = input.begin() + 1;
    while (current != input.end())
    {
        Digram digram = {*(current-1), *current};

        auto digramIt = digramIndex.find(digram);

        if (digramIt == digramIndex.end())
        {
            // new digram
            digramIndex[digram] = current;
            current++;
            continue;
        }

        // digram overlaps
        if (digramIt->second == current-1) {current++; continue;};

        auto ruleIt = ruleIndex.find(digram);
        if (ruleIt != ruleIndex.end())
        {
            // there exists a rule for the digram, we replace it
            *current = ruleIt->second;
            *(current-1) = std::numeric_limits<uint32_t>::max();
        }
        // there exists no rule for the digram, we replace both digrams by a new rule
        else {
            ruleIndex[digram] = base;
            *current = base;
            *(current-1) = std::numeric_limits<uint32_t>::max();
            *digramIt->second = base;
            *(digramIt->second - 1) = std::numeric_limits<uint32_t>::max();
            base++;
        }

        current++;
    }

    return {settings,input, {}};
}
