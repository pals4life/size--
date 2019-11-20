//============================================================================
// @name        : none.h
// @author      : Thomas Dooms
// @date        : 11/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <vector>
#include <algorithm>
#include <tuple>

#include "../util/variable.h"
#include "../util/production.h"
#include "../util/settings.h"

namespace algorithm::broken_bisection
{
    std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(const std::vector<uint8_t>& bytes){
        const auto settings = Settings();
        std::vector<Production> productions;
        std::vector<Variable> remaining;
        remaining.reserve(bytes.size() / 2 + 1);

	    for (auto it = bytes.begin(); it != bytes.end(); ++it)
	    {
	        const uint32_t temp0 = *it;
	        const uint32_t temp1 = *++it;
		    remaining.emplace_back(Settings::convert_to_reserved(temp0, temp1));
	    }

        return std::make_tuple(settings, std::move(remaining), std::move(productions));
    }
}

namespace algorithm::bisection
{
    std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(const std::vector<uint16_t>& pairs, bool odd){
        const auto settings = Settings();
        std::vector<Production> productions;

        std::vector<Variable> variables(pairs.begin(), pairs.end());
        std::for_each(variables.begin(), variables.end() - 1, [](auto& elem){ elem += 256; });

        if(not odd) variables.back() += 256;
        else variables.back() >>= 8u;

        return std::make_tuple(settings, std::move(variables), std::move(productions));
    }
}