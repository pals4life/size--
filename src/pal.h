//============================================================================
// @name        : compressed.h
// @author      : Thomas Dooms
// @date        : 11/14/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <vector>
#include <experimental/filesystem>

#include "util/settings.h"
#include "util/production.h"
#include "algorithms/type.h"
#include "util/variable.h"

namespace pal
{
    void encode(const std::experimental::filesystem::path& input, const std::experimental::filesystem::path& output, Algorithm type, bool tar = false);
    bool decode(const std::experimental::filesystem::path& input, const std::experimental::filesystem::path& output);

    std::vector<uint8_t> readBytes(const std::experimental::filesystem::path& path);
    std::pair<std::vector<uint16_t>, bool> readPairs(const std::experimental::filesystem::path& path);

    std::vector<uint8_t> calculateYield(const std::vector<Variable>& string, const std::vector<Production>& productions, Settings settings);
}
