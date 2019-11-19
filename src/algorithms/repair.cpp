//============================================================================
// @name        : repair.cpp
// @author      : Thomas Dooms
// @date        : 11/19/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <unordered_map>
#include <queue>
#include "repair.h"


std::tuple<Settings, std::vector<Variable>, std::vector<Production>> algorithm::repair::compress(const std::vector<uint8_t>& bytes)
{
    // vector of variables, can contains tombstones
    std::vector<Variable> variables(bytes.begin(), bytes.end());

    // positions of pair occurrences, queue can index into this vector
    std::vector<uint32_t> positions;

    // (variable0, variable1, start index, interval length, frequency
    std::priority_queue<std::tuple<Variable, Variable, uint32_t, uint32_t, uint32_t>> queue;

}