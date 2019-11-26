// =====================================================================
// @name: lzw.h
// @project: SIZE--
// @author: Mano Marichal
// @date: 25.11.19
// @copyright: BA2 Informatica - Mano Marichal - University of Antwerp
// @description: 
// =====================================================================

#pragma once
#include <vector>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <filesystem>
#include <tuple>
#include <fstream>

#include "../util/variable.h"
#include "../util/production.h"
#include "../util/settings.h"
#include "../util/settings.h"
#include "../bitio/bitreader.h"
#include "../bitio/bitwriter.h"
#include "../pal/metadata.h"

namespace algorithm::lzw
{

    std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(
            const std::filesystem::path& input,
            const std::filesystem::path& output)
    {


        // write metadata
        Settings settings;
        /*
        pal::Metadata metadata( , 0, settings);
        Bitwriter writer(output);
        pal::Encoder::encodeMetadata(writer, metadata);
        */

        // read input
        std::ifstream inputFile (input);
        if(not inputFile.is_open()) throw std::runtime_error("could not open file: " + output.string());
        std::ifstream outputFile (output);
        char c;
        inputFile.get(c);

        std::vector<Variable> variables;
        std::unordered_map<std::pair<uint16_t,uint16_t>, uint16_t, boost::hash<std::pair<uint16_t , uint16_t>>> table;
        table.reserve(4096);
        for (uint16_t i=0;i<256;i++) table[std::make_pair(i, 0)] = i;


        uint16_t value = settings.begin();
        uint16_t x = c;

        //compress
        while (inputFile.get(c))
        {
            auto pair = std::make_pair(x, c);
            auto it = table.find(pair);
            if (it != table.end())
            {
                x = it->second;
            }
            else {
                table[pair] = value;
                //outputFile << x;
                value++;
                x = c;
            }
        }
        //outputFile << x;
        return std::make_tuple(settings, std::vector<Variable>(), std::vector<Production>());
    }
}

