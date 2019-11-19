//============================================================================
// @name        : encodeMetadata.h
// @author      : Thomas Dooms
// @date        : 11/15/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <bitset>
#include <cmath>
#include <array>

class Settings
{
public:
    enum Flags : size_t
    {
        noflags = 0u,
        reserved = 1u,
        variable_length = 2u,
    };

    Settings(uint8_t flags = Flags::reserved) : flags(flags)
    {
        charLength = std::floor(std::log2(begin() + productionSize));
    }

    [[nodiscard]] uint32_t convert_to_reserved(uint32_t first, uint32_t second)
    {
        return (first << 8u) + second;
    }
    [[nodiscard]] std::array<uint32_t, 2> convert_from_reserved(uint32_t index)
    {
        return {index << 8u, index & 0x0000000Fu};
    }

    [[nodiscard]] bool is_reserved_rule(uint32_t first, uint32_t second)
    {
        return first < byte_end and second < byte_end;
    }

    [[nodiscard]] uint32_t offset(uint32_t index) const noexcept
    {
        return begin() + index;
    }
    [[nodiscard]] uint32_t begin() const noexcept
    {
        return (flags & Flags::reserved) ? reserved_end : byte_end;
    }
    [[nodiscard]] uint32_t end() const noexcept
    {
        return std::numeric_limits<uint32_t>::max();
    }

    [[nodiscard]] static bool is_byte(uint32_t index)
    {
        return index < byte_end;
    }
    [[nodiscard]] bool is_reserved_variable(uint32_t index)
    {
        return has_reserved() and index >= byte_end and index < reserved_end;
    }
    [[nodiscard]] bool is_valid(uint32_t index)
    {
        return index >= begin() and index < end();
    }
    [[nodiscard]] bool has_reserved() const noexcept
    {
        return flags & reserved;
    }
    [[nodiscard]] bool has_variable_length() const noexcept
    {
        return flags & variable_length;
    }

    uint32_t stringSize;
    uint32_t productionSize;
    uint8_t charLength;
    uint8_t flags;

private:
    constexpr inline static uint32_t byte_end = 256;
    constexpr inline static uint32_t reserved_end = 256 + 256 * 256;
};