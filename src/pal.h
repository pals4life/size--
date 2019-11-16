//============================================================================
// @name        : pal.h
// @author      : Thomas Dooms
// @date        : 11/14/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <filesystem>
#include <vector>

#include "pal/metadata.h"
#include "util/production.h"
#include "algorithms/type.h"

namespace pal
{
    void encode(const std::filesystem::path& input, const std::filesystem::path& output, Algorithm type);
    void decode(const std::filesystem::path& input, const std::filesystem::path& output);

    std::string readFile(const std::filesystem::path& path);
    std::string calculateYield(const std::vector<uint32_t>& string, const std::vector<Production>& productions, Metadata settings);
}
