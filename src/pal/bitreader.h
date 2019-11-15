//============================================================================
// @name        : bitreader.h
// @author      : Thomas Dooms
// @date        : 11/14/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <fstream>
#include <filesystem>

class Bitreader
{
public:
    explicit Bitreader(const std::filesystem::path& path)
    {
        file = std::ifstream(path, std::ios::binary);
        read_buffer();
        index = 0;
    }

    template<typename Type = uint64_t>
    Type read_value(uint8_t size)
    {
        Type result;
        if(index + size > 64)
        {
            const auto shift = buffer_size - index;
            result = buffer << index;
            result <<= size - shift;

            read_buffer();

            result += buffer >> (buffer_size - size + shift);
            index = size - shift;
        }
        else
        {
            result = (buffer << index) >> (buffer_size - size);
            index += size;
        }
        return result;
    }

    template<typename Type>
    Type read_value()
    {
        return read_value(sizeof(Type) * 8);
    }

    bool read_bit()
    {
        if(index == 64)
        {
            read_buffer();
            index = 0;
        }
        index++;
        return (buffer >> (buffer_size - index)) & 1u;
    }

    void read_buffer()
    {
        file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
    }


private:
    std::ifstream file;
    uint64_t buffer;
    uint64_t index;

    constexpr inline static auto buffer_size = sizeof(buffer) * 8;
};