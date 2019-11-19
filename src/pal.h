//============================================================================
// @name        : pal.h
// @author      : Thomas Dooms
// @date        : 11/14/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <vector>

#include "util/settings.h"
#include "util/production.h"
#include "algorithms/type.h"
#include "util/variable.h"

namespace pal
{
    void encode(const std::string& input, const std::string& output, Algorithm type);
    void decode(const std::string& input, const std::string& output);

    std::vector<uint8_t> readFile(const std::string& path);
    std::vector<uint8_t> calculateYield(const std::vector<Variable>& string, const std::vector<Production>& productions, Settings settings);
}
