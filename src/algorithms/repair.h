//============================================================================
// @name        : repair.h
// @author      : Thomas Dooms
// @date        : 11/19/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <tuple>
#include <vector>

#include "../util/settings.h"
#include "../util/variable.h"
#include "../util/production.h"


namespace algorithm::repair
{
    std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(const std::vector<uint8_t>& bytes);
}



