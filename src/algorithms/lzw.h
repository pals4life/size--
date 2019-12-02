// =====================================================================
// @name: lzw.h
// @project: SIZE--
// @author: Mano Marichal
// @date: 25.11.19
// @copyright: BA2 Informatica - Mano Marichal - University of Antwerp
// @description: 
// =====================================================================

//#pragma once
//#include <vector>
//#include <unordered_map>
//#include <boost/functional/hash.hpp>
//#include <experimental/filesystem>
//#include <tuple>
//#include <fstream>
//#include "../util/robin_hood.h"
//
//
//#include "../util/variable.h"
//#include "../util/production.h"
//#include "../util/settings.h"
//#include "../util/settings.h"
//#include "../bitio/bitreader.h"
//#include "../bitio/bitwriter.h"
//#include "../pal/metadata.h"
//
//namespace algorithm::lzw
//{
//
//    std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(
//            const std::experimental::filesystem::path& input,
//            const std::experimental::filesystem::path& output)
//    {
//
//
//        using Pair = uint32_t;
//        constexpr auto compose   = [](Variable lhs, Variable rhs) -> Pair { return (static_cast<Pair>(lhs) << 16u) + rhs; };
//
//
//        // write metadata
//        Settings settings;
//
//        pal::Metadata metadata(0 , 0, settings);
//        Bitwriter writer(output);
//        pal::Encoder::encodeMetadata(writer, metadata);
//
//
//        // read input
//        std::ifstream inputFile (input);
//        if(not inputFile.is_open()) throw std::runtime_error("could not open file: " + output.string());
//        std::ifstream outputFile (output);
//        char c;
//        inputFile.get(c);
//
//        std::vector<Variable> variables;
//        robin_hood::unordered_flat_map <uint32_t , uint16_t> map;
//        for (uint16_t i=0;i<256;i++) map[compose(i, 0)] = i;
//
//
//        uint16_t value = settings.begin();
//        uint16_t x = c;
//
//
//        //compress
//        while (inputFile.get(c))
//        {
//            auto pair = compose(x, c);
//            auto it = map.find(compose(x, c));
//            if (it != map.end())
//            {
//                x = it->second;
//            }
//            else {
//                map[pair] = value;
//                value++;
//                x = c;
//            }
//        }
//
//        //outputFile << x;
//        return std::make_tuple(settings, std::vector<Variable>(), std::vector<Production>());
//    }
//}

