//============================================================================
// @name        : pal.cpp
// @author      : Thomas Dooms
// @date        : 11/14/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include <cmath>
#include <numeric>
#include "pal.h"

namespace pal
{

void encode(const std::filesystem::path& input, const std::vector<uint32_t>& string, const std::vector<Production>& productions)
{
    Settings settings(string.size(), productions.size(), Settings::Flags::reserve);
    HuffmanEncoder encoder(string, productions, settings);
    Bitwriter writer(input);

    pal::encodeSettings(writer, settings);
    pal::encodeHuffmanTree(writer, encoder.root, settings);
    pal::encodeProductions(writer, encoder, productions);
    pal::encodeString(writer, encoder, string);
}

void decode(const std::filesystem::path& input, const std::filesystem::path& output)
{
    Bitreader reader(input);

    const auto settings = pal::decodeSettings(reader);

    auto root = pal::decodeHuffmanTree(reader, settings);
    HuffmanDecoder decoder(std::move(root));

    const auto productions = pal::decodeProductions(reader, decoder, settings);
    const auto string = pal::decodeString(reader, decoder, settings);

    std::ofstream file(output);
    file << pal::calculateYield(string, productions, settings);
}

// ------------------------------------------------------- //

void encodeSettings(Bitwriter& writer, Settings settings)
{
    writer.write_value(settings.stringSize);
    writer.write_value(settings.productionSize);
    writer.write_value(settings.flags);
}

void encodeHuffmanTree(Bitwriter& writer, const std::unique_ptr<Huffman::Node>& root, Settings settings)
{
    if(root->leaf())
    {
        writer.write_bit(true);
        writer.write_value(root->character, settings.charLength);
    }
    else
    {
        writer.write_bit(false);
        encodeHuffmanTree(writer, root->left, settings);
        encodeHuffmanTree(writer, root->right, settings);
    }
}

void encodeProductions(Bitwriter& writer, HuffmanEncoder& encoder, const std::vector<Production>& productions)
{
    for(const auto production : productions)
    {
        encoder.encode_character(writer, production.body[0]);
        encoder.encode_character(writer, production.body[1]);
    }
}

void encodeString(Bitwriter& writer, const HuffmanEncoder& encoder, const std::vector<uint32_t>& string)
{
    for(const auto character : string)
    {
        encoder.encode_character(writer, character);
    }
}

// ------------------------------------------------------- //

Settings decodeSettings(Bitreader& reader)
{
    const auto stringSize = reader.read_value<uint32_t>();
    const auto productionSize = reader.read_value<uint32_t>();
    const auto flags = reader.read_value<uint8_t>();

    return Settings(stringSize, productionSize, flags);
}

std::unique_ptr<Huffman::Node> decodeHuffmanTree(Bitreader& reader, Settings settings)
{
    if(reader.read_bit())
    {
        const auto character = reader.read_value(settings.charLength);
        return std::make_unique<Huffman::Node>(character);
    }
    else
    {
        auto node = std::make_unique<Huffman::Node>();
        node->left  = decodeHuffmanTree(reader, settings);
        node->right = decodeHuffmanTree(reader, settings);
        node->character = std::numeric_limits<uint32_t>::max();
        return node;
    }
}

std::vector<Production> decodeProductions(Bitreader& reader, HuffmanDecoder& decoder, Settings settings)
{
    std::vector<Production> result(settings.productionSize);
    for(size_t i = 0; i < settings.productionSize; i++)
    {
        result[i].head    = settings.start() + i;
        result[i].body[0] = decoder.decode_character(reader);
        result[i].body[1] = decoder.decode_character(reader);
    }
    return result;
}

std::vector<uint32_t> decodeString(Bitreader& reader, HuffmanDecoder& decoder, Settings settings)
{
    std::vector<uint32_t> result(settings.stringSize);
    for(size_t i = 0; i < settings.stringSize; i++)
    {
        result[i] = decoder.decode_character(reader);
    }
    return result;
}

std::string calculateYield(const std::vector<uint32_t>& string, const std::vector<Production>& productions, Settings settings)
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
    std::string result(size, '\000');

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