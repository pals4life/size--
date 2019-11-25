// =====================================================================
// @name: lzw.h
// @project: SIZE--
// @author: Mano Marichal
// @date: 25.11.19
// @copyright: BA2 Informatica - Mano Marichal - University of Antwerp
// @description: 
// =====================================================================

#pragma once
#include <vector>
#include <unordered_map>
#include <boost/functional/hash.hpp>

#include "../util/variable.h"
#include "../util/production.h"
#include "../util/settings.h"
#include <tuple>

namespace algorithm::lzw
{
    std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(const std::vector<uint8_t>& bytes)
    {
        std::string input, x, y;
        input.reserve(bytes.size());
        for (auto c:bytes) input += std::string(1, c);

        std::vector<Variable> variables;
        std::unordered_map<std::string, uint32_t> table;
        for (uint32_t i=0;i<256;i++) table[std::string(1, i)] = i;

        Settings settings;
        uint32_t value = settings.begin();
        value = 256;

        x = input[0];
        for (int i = 0; i < input.length(); i++)
        {
            if (i < input.length()-1) y = input[i+1];
            if (table.find(x+y) != table.end()) x += y;
            else {
                table[x+y] = value;
                variables.emplace_back(table.at(x));
                value++;
                x = y;
            }
            y = "";
        }
        variables.emplace_back(table[x]);

        return {settings, std::move(variables), {}};
    }
}

