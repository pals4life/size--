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

#include "../util/variable.h"
#include "../util/production.h"

// ik zet de bytes gewoon om naar een vector van variabelen en zonder producties

// maak gewoon een bestand zoals dit aan met uw code erin en deze function signature

namespace algorithm::none
{
    std::pair<std::vector<Variable>, std::vector<Production>> compress(const std::vector<uint8_t>& bytes)
    {
        std::vector<Variable> vec(bytes.begin(), bytes.end());
        return std::make_pair(std::move(vec), std::vector<Production>());
    }
}