//============================================================================
// @name        : production.h
// @author      : Thomas Dooms
// @date        : 11/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <array>

struct Production
{
    uint32_t head;
    std::array<uint32_t, 2> body;
};