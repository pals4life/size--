//============================================================================
// @name        : compressed.cpp
// @author      : Thomas Dooms
// @date        : 11/14/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <numeric>

#include "pal.h"

#include "algorithms/none.h"
#include "algorithms/sequitur/sequitur.h"

#include "pal/decoder.h"
#include "pal/encoder.h"
#include "algorithms/bisection.h"
#include "algorithms/repair.h"
#include "algorithms/lzw.h"


namespace pal
{

void encode(const std::experimental::filesystem::path& input, const std::experimental::filesystem::path& output, Algorithm type, bool tar)
{

    auto [settings, string, productions] = [&]()
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
        else if(type == Algorithm::bisectionPlusPlus)
        {
            const auto [pairs, odd] = readPairs(input);
            return algorithm::bisectionPlusPlus::compress(pairs, odd);
        }
        else if(type == Algorithm::bisectionPlusPlusPlusPlus)
        {
            const auto [pairs, odd] = readPairs(input);
            return algorithm::bisectionPlusPlusPlusPlus::compress(pairs, odd);
        }
        else if(type == Algorithm::lzw)
        {
            algorithm::lzw::compress(input, output);
            exit(0);
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

    if(tar) settings.flags |= Settings::Flags::tar;

    Metadata metadata(string.size(), productions.size(), settings);
    pal::Encoder::encode(output, string, productions, metadata);
}

bool decode(const std::experimental::filesystem::path& input, const std::experimental::filesystem::path& output)
{
    const auto [metadata, productions, string] = Decoder::decode(input);

    std::ofstream file(output, std::ios::binary);
    if(not file.is_open()) throw std::runtime_error("could not open file: " + output.string());
    const auto yield = calculateYield(string, productions, metadata.settings);

    file.write(reinterpret_cast<const char*>(yield.data()), yield.size() * sizeof(uint8_t));
    return metadata.settings.is_tar();
}

// ------------------------------------------------------- //

std::vector<uint8_t> readBytes(const std::experimental::filesystem::path& path)
{
    auto file = fopen(path.c_str(), "rb");
    if(not file) throw std::runtime_error("could not open file: " + path.string());
    fseek(file, 0, SEEK_END);
    const auto size = static_cast<size_t>(ftell(file));
    fseek(file, 0, SEEK_SET);

    std::vector<uint8_t> string(size);
    fread(string.data(), 1, size, file);

    fclose(file);
    return string;
}

std::pair<std::vector<uint16_t>, bool> readPairs(const std::experimental::filesystem::path& path)
{
    auto file = fopen(path.c_str(), "rb");
    if(not file) throw std::runtime_error("could not open file: " + path.string());
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

    const auto calcSize = [&](auto base, auto index) -> size_t
    {
        if (Settings::is_byte(index)) return base + 1;
        else if (settings.is_reserved_variable(index)) return base + 2;
        else return base + yields[index - settings.begin()].size();
    };

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

        yields[i].reserve(calcSize(calcSize(0, productions[i][0]), productions[i][1]));
        evaluate(productions[i][0]);
        evaluate(productions[i][1]);
    }

    const auto size = std::accumulate(string.begin(), string.end(), 0ul, calcSize);
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
