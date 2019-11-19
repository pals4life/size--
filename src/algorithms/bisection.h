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

namespace algorithm::bisection
{
    std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(const std::vector<uint8_t>& bytes){
        const auto settings = Settings();
        std::vector<Production> productions;
        std::vector<Variable> remaining;

	    for (auto it = bytes.begin(); it != bytes.end(); ++it) {
		    remaining.emplace_back(Settings::convert_to_reserved(*it, *(++it)));
	    }

        return std::make_tuple(settings, std::move(remaining), std::move(productions));
    }
}