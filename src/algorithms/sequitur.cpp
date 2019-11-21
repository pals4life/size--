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


void algorithm::sequitur::decode(const std::unordered_map<Digram, uint32_t, boost::hash<Digram>> &rules, u_int32_t c, u_int32_t begin)
{
    if (c == std::numeric_limits<uint32_t>::max()) return;

    if (c < begin)
    {
        std::cout << char(c) << std::flush;
        return;
    }

    for (auto &rule:rules)
    {
        if (rule.second == c)
        {
            decode(rules, rule.first.first, begin);
            decode(rules, rule.first.second, begin);
            return;
        }
    }
}


std::vector<uint32_t>::iterator findPreviousSymbolIt(std::vector<uint32_t>::iterator it, const std::vector<uint32_t> &vec)
{
    if (it == vec.begin()) return it;
    it--;
    while (*it == std::numeric_limits<uint32_t>::max())
    {
        it--;
        if (it == vec.begin()) break;

    }
    return it;
}


uint32_t findPreviousSymbol(std::vector<uint32_t>::iterator it, const std::vector<uint32_t> &vec)
{
    if (it == vec.begin()) return std::numeric_limits<uint32_t>::max();
    it--;
    while (*it == std::numeric_limits<uint32_t>::max())
    {
        it--;
        if (it == vec.begin()) return std::numeric_limits<uint32_t>::max();

    }

    return *it;
}

uint32_t findNextSymbol(std::vector<uint32_t>::iterator it, const std::vector<uint32_t> &vec)
{
    it++;
    if (it == vec.end()) return std::numeric_limits<uint32_t>::max();

    while (*it == std::numeric_limits<uint32_t>::max())
    {
        it++;
        if (it == vec.end()) return std::numeric_limits<uint32_t>::max();

    }

    return *it;
}

// TODO: nieuw gevormde digrams, rule utility, thomas adt muur afstemmen
std::tuple<Settings, std::vector<Variable>, std::vector<Production>> algorithm::sequitur::compress(
        std::vector<unsigned char> string)
{
    Settings settings(Settings::Flags::noflags);

    uint32_t maxVal = std::numeric_limits<uint32_t>::max();
    u_int32_t base = settings.begin();

    std::unordered_map<Digram, TableValue , boost::hash<Digram>> digramIndex;
    std::unordered_map<Digram , uint32_t, boost::hash<Digram>> ruleIndex;

    std::vector<uint32_t> input;
    input.insert(input.end(),string.begin(), string.end());

    std::vector<uint32_t> ::iterator current = input.begin() + 1;

    while (current != input.end())
    {
        Digram digram = {*(current-1), *current};
        auto digramIt = digramIndex.find(digram);

        if (digramIt == digramIndex.end())
        {
            // new digram
            digramIndex.insert({digram,TableValue(current)});
            current++;
            continue;
        }

        // digram overlaps
        if (digramIt->second.location == current-1) {current++; continue;};

        //check if a rule exists, if so replace the digram
        auto ruleIt = ruleIndex.find(digram);

        if (ruleIt != ruleIndex.end())
        {
            *current = ruleIt->second;
            *(findPreviousSymbolIt(current, input)) = maxVal;

            // delete the overlapping digram
            digramIndex.erase({findPreviousSymbol(current, input), digram.first});
            // add the correct new digram
            digramIndex.insert({{findPreviousSymbol(current, input), ruleIt->second},
                    TableValue(current)});

            digramIndex.erase(digram);

        }
        // there exists no rule for the digram, we replace both digrams by a new rule
        else {

            ruleIndex[digram] = base;

            uint32_t curLSymbol = findPreviousSymbol(current-1, input);
            *current = base;
            *(findPreviousSymbolIt(current,input)) = maxVal;


            // delete the overlapping digram
            digramIndex.erase({curLSymbol, digram.first});
            // add the correct new digram
            digramIndex.insert({{curLSymbol, base},
                                TableValue(current)});

            uint32_t digramLSymbol = findPreviousSymbol(digramIt->second.location-1, input);
            uint32_t digramRSymbol = findNextSymbol(digramIt->second.location-1, input);

            //replace digram 2
            *(digramIt->second.location) = base;
            *(findPreviousSymbolIt(digramIt->second.location, input)) = maxVal;

            if (digramLSymbol != maxVal)
            {
                digramIndex.erase({digramIt->first.first, digramLSymbol});
                digramIndex.insert({{digramLSymbol, base},
                        TableValue(current)});
            }

            if (digramRSymbol != maxVal)
            {
                digramIndex.erase({digramIt->first.second, digramRSymbol});
                digramIndex.insert({{base, digramRSymbol},
                                    TableValue(current)});
            }


            base++;
            digramIndex.erase(digram);

        }

        current++;
    }

    return {settings,input, {}};

}
/*
std::tuple<Settings, std::vector<Variable>, std::vector<Production>> algorithm::sequitur::compress(
        std::vector<unsigned char> string)
{
    Settings settings(Settings::Flags::noflags);

    u_int32_t base = settings.begin();

    std::unordered_map<Digram, uint32_t, boost::hash<Digram>> ruleIndex;
    std::map<Digram, std::vector<uint32_t>::iterator> digramIndex;

    std::vector<uint32_t> input(string.begin(), string.end());

    auto current = input.begin() + 1;
    while (current != input.end())
    {
        Digram digram = {*(current-1), *current};
        auto digramIt = digramIndex.find(digram);
        if (digramIt == digramIndex.end())
        {
            // new digram
            digramIndex.insert({digram, current});
            current++;
            continue;
        }
        // digram overlaps
        if (digramIt->second == current-1) {current++; continue;};
        //check if a rule exists, if so replace the digram
        auto ruleIt = ruleIndex.find(digram);
        if (ruleIt != ruleIndex.end()) replaceDigram(current, ruleIt->second, digram, digramIt,digramIndex);
            // there exists no rule for the digram, we replace both digrams by a new rule
        else {
            ruleIndex.insert({digram, base});
            replaceDigram(current, base, digram,digramIt,digramIndex);
            replaceDigram(digramIt->second, base, digram, digramIt, digramIndex);
            base++;
        }

        current++;
    }

    for (auto c:input)
    {
        decode(ruleIndex, c,settings.begin());
    }
    return {settings,input, {}};
}
*/

/*
// TODO: nieuw gevormde digrams, rule utility, thomas adt muur afstemmen
std::tuple<Settings, std::vector<Variable>, std::vector<Production>> algorithm::sequitur::compress(
        std::vector<unsigned char> string)
{
    Settings settings(Settings::Flags::noflags);

    uint32_t maxVal = std::numeric_limits<uint32_t>::max();
    u_int32_t base = settings.begin();

    using digramIndexValue = std::tuple<std::vector<uint32_t>::iterator, Digram, Digram>;
    using digramIt = std::vector<Digram>::const_iterator;

    std::unordered_map<Digram*, uint32_t, boost::hash<Digram*>> ruleIndex;
    std::unordered_map<Digram , digramIndexValue , boost::hash<Digram>> digramIndex;
    std::vector<Digram> digrams;

    std::vector<uint32_t> input(maxVal, 1);
    input.insert(input.end(),string.begin(), string.end());

    auto current = input.begin() + 1;
    while (current != input.end())
    {
        Digram digram = {*(current-1), *current};
        auto digramIt = digramIndex.find(digram);

        if (digramIt == digramIndex.end())
        {
            // new digram
            digrams.emplace_back(digram);
            digramIndex.insert({digrams.end(), current; {}});
            current++;
            continue;
        }
        // digram overlaps
        if (digramIt->second == current-1) {current++; continue;};
        //check if a rule exists, if so replace the digram
        auto ruleIt = ruleIndex.find(digram);
        if (ruleIt != ruleIndex.end()) replaceDigram(current, ruleIt->second, digram, digramIndex);
            // there exists no rule for the digram, we replace both digrams by a new rule
        else {
            ruleIndex.insert({digram, base});
            replaceDigram(current, base, digram, digramIndex);
            replaceDigram(digramIt->second, base, digram, digramIndex);
            base++;
        }

        current++;
    }

    for (auto c:input)
    {
        decode(ruleIndex, c,settings.begin());
    }

    return {settings,input, {}};
}
*/