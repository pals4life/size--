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
    std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(const std::vector<uint8_t>& bytes)
    {
        std::vector<Variable> vec(bytes.begin(), bytes.end());
        const auto settings = Settings(Settings::Flags::noflags);

        return std::make_tuple(settings, std::move(vec), std::vector<Production>());
    }
}