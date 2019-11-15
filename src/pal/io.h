//============================================================================
// @name        : read.h
// @author      : Thomas Dooms
// @date        : 11/15/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <filesystem>
#include <fstream>

namespace pal
{
    std::string readFile(const std::filesystem::path& path)
    {
        auto file = fopen(path.c_str(), "rb");
        if(not file) throw std::runtime_error("could not open file: " + path.string());
        fseek(file, 0, SEEK_END);
        auto size = static_cast<size_t>(ftell(file));
        fseek(file, 0, SEEK_SET);

        std::string string(size, 0);
        fread(string.data(), 1, size, file);
        fclose(file);
        return string;
    }
}