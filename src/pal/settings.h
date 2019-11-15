//============================================================================
// @name        : encodeSettings.h
// @author      : Thomas Dooms
// @date        : 11/15/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <bitset>
#include <cmath>

class Settings
{
public:
    enum Flags : size_t
    {
        noflags = 0u,
        reserve = 1u,
        variable = 2u,
    };

    Settings(uint32_t stringSize, uint32_t productionSize, uint8_t flags) : stringSize(stringSize), productionSize(productionSize), flags(flags)
    {
        charLength = std::floor(std::log2(start() + productionSize));
    }

    [[nodiscard]] uint32_t start() const noexcept
    {
        if(flags & Flags::reserve) return reserved_end;
        else return byte_end;
    }

    [[nodiscard]] bool reserved() const noexcept
    {
        return flags & reserve;
    }

    uint32_t stringSize;
    uint32_t productionSize;
    uint8_t charLength;
    uint8_t flags;

private:
    constexpr inline static uint32_t byte_end = 256;
    constexpr inline static uint32_t reserved_end = 256 + 256 * 256;
};