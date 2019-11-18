//============================================================================
// @name        : pal.cpp
// @author      : Thomas Dooms
// @date        : 11/14/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <numeric>
#include "pal.h"

#include "algorithms/none.h"

#include "pal/decoder.h"
#include "pal/encoder.h"


namespace pal
{

void encode(const std::filesystem::path& input, const std::filesystem::path& output, Algorithm type)
{
    const auto [string, productions] = [&]()
    {
        const auto bytes = readFile(input);
        switch(type)
        {
            case Algorithm::none:
                return algorithm::none::compress(bytes);

            default:
                throw std::runtime_error("unknown algorithm");
        }
    }();

    Metadata settings(string.size(), productions.size(), Metadata::Flags::noflags);
    pal::Encoder::encode(output, string, productions, settings);
}

void decode(const std::filesystem::path& input, const std::filesystem::path& output)
{
    const auto [settings, productions, string] = Decoder::decode(input);

    std::ofstream file(output, std::ios::binary);
    if(not file.is_open()) throw std::runtime_error("could not open file: " + output.string());
    const auto yield = calculateYield(string, productions, settings);
    file.write(reinterpret_cast<const char*>(&yield), yield.size() * sizeof(uint8_t));
}

// ------------------------------------------------------- //

std::vector<uint8_t> readFile(const std::filesystem::path& path)
{
    auto file = fopen(path.c_str(), "rb");
    if(not file) throw std::runtime_error("could not open file: " + path.string());
    fseek(file, 0, SEEK_END);
    auto size = static_cast<size_t>(ftell(file));
    fseek(file, 0, SEEK_SET);

    std::vector<uint8_t> string(size, 0);
    fread(string.data(), 1, size, file);
    fclose(file);
    return string;
}

std::vector<uint8_t> calculateYield(const std::vector<Variable>& string, const std::vector<Production>& productions, Metadata settings)
{
    std::vector<std::string> yields(productions.size());

    for(size_t i = 0; i < productions.size(); i++)
    {
        const auto evaluate = [&](auto index)
        {
            if(index < 256)
            {
                yields[i] += index;
            }
            else if(settings.reserved() and index < settings.start())
            {
                yields[i] += index >> 8u;     // bits 15 - 8
                yields[i] += index & 0x000Fu; // bits 7  - 0
            }
            else
            {
                yields[i] += yields[index];
            }
        };

        evaluate(productions[i].body[0]);
        evaluate(productions[i].body[1]);
    }

    const auto func = [&](auto base, auto index) -> size_t
    {
        if(index < 256) return base + 1;
        else if(settings.reserved() and index < settings.start()) return base + 2;
        else return base + yields[index - settings.start()].size();
    };

    const auto size = std::accumulate(string.begin(), string.end(), 0ul, func);
    std::vector<uint8_t> result(size, '\000');

    auto iter = result.begin();
    for(const auto index : string)
    {
        if(index < 256)
        {
            *iter++ = index;
        }
        else if(settings.reserved() and index < settings.start())
        {
            *iter++ = index >> 8u;
            *iter++ = index & 0x000Fu;
        }
        else
        {
            const auto& yield = yields[index - settings.start()];
            iter = std::copy(yield.begin(), yield.end(), iter);
        }
    }

    return result;
}

}
