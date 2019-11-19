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
std::tuple<Settings, std::vector<Variable>, std::vector<Production>> algorithm::sequitur::compress(
        std::vector<unsigned char> string, uint32_t base)
{
    std::list<uint32_t> input(string.begin(), string.end());
    std::vector<Digram> digramIndex;
    std::vector<Rule> ruleIndex;

    auto current=++input.begin();
    auto previous=input.begin();

    // loop over each symbol except the first one
    while (current != input.end())
    {

        Digram tempDigram = {(*current), (*previous)};

        // check if an existing rule matches the digram
        bool ruleExists = false;
        for (auto &rule: ruleIndex)
        {
            if (rule.body == tempDigram)
            {;
                *current = rule.head;
                input.erase(previous);
                previous = current;
                previous--;
                ruleExists = true;
            }
        }

        // check if we previous encountered this diagram
        if (!ruleExists)
        {
            auto newIt = ++input.begin();
            auto newPrevIt = input.begin();

            // we can only look for digrams before the symbol and where it does not overlap
            while (newIt != current)
            {
                if (newIt == previous) break; // overlap
                Digram digram = {(*newIt), (*newPrevIt)};

                if (tempDigram == digram)
                {

                    // add a new rule for this digram and replace both instances
                    ruleIndex.emplace_back(Rule(base, digram));

                    //replace both digram instances by the new rule
                    *current = base;

                        input.erase(previous);
                    previous = current;
                    previous--;

                    *newIt = base;
                    input.erase(newPrevIt);

                    base++; // increase symbol used for next rule
                    break;
                }
                newIt++;
                newPrevIt++;
            }
        }
        current++;
        previous++;
    }
    
    // preparing return output
    std::vector<Production> productions;
    std::vector<Variable > variables;

    for (auto c:input)
    {
        variables.emplace_back(c);
    }

    for (const auto &rule:ruleIndex)
    {
        Production temp;
        temp.body = {rule.body.first, rule.body.second};
        productions.emplace_back(temp);
    }

    return {Settings().begin(),variables, productions};
}