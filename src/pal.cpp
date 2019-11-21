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
#include "algorithms/sequitur.h"

#include "pal/decoder.h"
#include "pal/encoder.h"
#include "algorithms/bisection.h"
#include "algorithms/repair.h"


namespace pal
{

void encode(const std::string& input, const std::string& output, Algorithm type)
{
    const auto [settings, string, productions] = [&]()
    {
        if(type == Algorithm::none)
        {
            const auto bytes = readBytes(input);
            return algorithm::none::compress(bytes);
        }
        else if(type == Algorithm::sequitur)
        {
            const auto bytes = readBytes(input);
            return algorithm::sequitur::compress(bytes);
        }
        else if(type == Algorithm::bisection)
        {
            const auto [pairs, odd] = readPairs(input);
            return algorithm::bisection::compress(pairs, odd);
        }
        else if(type == Algorithm::repair)
        {
            const auto bytes = readBytes(input);
            return algorithm::repair::compress(bytes);
        }
        else
        {
            throw std::runtime_error("unknown algorithm");
        }
    }();

    Metadata metadata(string.size(), productions.size(), settings);
    pal::Encoder::encode(output, string, productions, metadata);
}

void decode(const std::string& input, const std::string& output)
{
    const auto [metadata, productions, string] = Decoder::decode(input);

    std::ofstream file(output, std::ios::binary);
    if(not file.is_open()) throw std::runtime_error("could not open file: " + output);
    const auto yield = calculateYield(string, productions, metadata.settings);

    file.write(reinterpret_cast<const char*>(yield.data()), yield.size() * sizeof(uint8_t));
}

// ------------------------------------------------------- //

std::vector<uint8_t> readBytes(const std::string& path)
{
    auto file = fopen(path.c_str(), "rb");
    if(not file) throw std::runtime_error("could not open file: " + path);
    fseek(file, 0, SEEK_END);
    const auto size = static_cast<size_t>(ftell(file));
    fseek(file, 0, SEEK_SET);

    std::vector<uint8_t> string(size);
    fread(string.data(), 1, size, file);

    fclose(file);
    return string;
}

std::pair<std::vector<uint16_t>, bool> readPairs(const std::string& path)
{
    auto file = fopen(path.c_str(), "rb");
    if(not file) throw std::runtime_error("could not open file: " + path);
    fseek(file, 0, SEEK_END);
    const auto size = static_cast<size_t>(ftell(file));
    fseek(file, 0, SEEK_SET);

    std::vector<uint16_t> string((size / 2) + (size % 2));
    fread(string.data(), 2, size, file);

    fclose(file);
    return std::make_pair(std::move(string), size % 2);
}

std::vector<uint8_t> calculateYield(const std::vector<Variable>& string, const std::vector<Production>& productions, Settings settings)
{
    std::vector<std::string> yields(productions.size());

    for(size_t i = 0; i < productions.size(); i++)
    {
        const auto evaluate = [&](auto index)
        {
            if(Settings::is_byte(index))
            {
                yields[i] += index;
            }
            else if(settings.is_reserved_variable(index))
            {
                const auto [var0, var1] = Settings::convert_from_reserved(index);
                yields[i] += var0;
                yields[i] += var1;
            }
            else
            {
                yields[i] += yields[index - settings.begin()];
            }
        };

        evaluate(productions[i][0]);
        evaluate(productions[i][1]);
    }

    const auto func = [&](auto base, auto index) -> size_t
    {
        if(Settings::is_byte(index)) return base + 1;
        else if(settings.is_reserved_variable(index)) return base + 2;
        else return base + yields[index - settings.begin()].size();
    };

    const auto size = std::accumulate(string.begin(), string.end(), 0ul, func);
    std::vector<uint8_t> result(size);

    auto iter = result.begin();
    for(const auto index : string)
    {
        if(Settings::is_byte(index))
        {
            *iter++ = index;
        }
        else if(settings.is_reserved_variable(index))
        {
            const auto [var0, var1] = Settings::convert_from_reserved(index);
            *iter++ = var0;
            *iter++ = var1;
        }
        else
        {
            const auto& yield = yields[index - settings.begin()];
            iter = std::copy(yield.begin(), yield.end(), iter);
        }
    }

    return result;
}

}
