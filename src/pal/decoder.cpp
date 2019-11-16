//============================================================================
// @name        : palDecoder.cpp
// @author      : Thomas Dooms
// @date        : 11/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "decoder.h"

namespace pal
{

std::tuple<Metadata, std::vector<Production>, std::vector<Variable>> Decoder::decode(const std::filesystem::path& path)
{
    Bitreader reader(path);

    const auto settings = decodeMetadata(reader);

    auto root = decodeHuffmanTree(reader, settings);
    huffman::Decoder decoder(std::move(root));

    auto productions = decodeProductions(reader, decoder, settings);
    auto string = decodeString(reader, decoder, settings);

    return {settings, std::move(productions), std::move(string)};
}

Metadata Decoder::decodeMetadata(Bitreader& reader)
{
    const auto stringSize = reader.read_value<uint32_t>();
    const auto productionSize = reader.read_value<uint32_t>();
    const auto flags = reader.read_value<uint8_t>();

    return Metadata(stringSize, productionSize, flags);
}

std::unique_ptr<huffman::Node> Decoder::decodeHuffmanTree(Bitreader& reader, Metadata settings)
{
    if(reader.read_bit())
    {
        const auto character = reader.read_value(settings.charLength);
        return std::make_unique<huffman::Node>(character);
    }
    else
    {
        auto node = std::make_unique<huffman::Node>();
        node->left  = decodeHuffmanTree(reader, settings);
        node->right = decodeHuffmanTree(reader, settings);
        node->character = std::numeric_limits<uint32_t>::max();
        return node;
    }
}

std::vector<Production> Decoder::decodeProductions(Bitreader& reader, huffman::Decoder& decoder, Metadata settings)
{
    std::vector<Production> result(settings.productionSize);
    for(size_t i = 0; i < settings.productionSize; i++)
    {
        result[i].head    = settings.start() + i;
        result[i].body[0] = decoder.decodeVariable(reader);
        result[i].body[1] = decoder.decodeVariable(reader);
    }
    return result;
}

std::vector<Variable> Decoder::decodeString(Bitreader& reader, huffman::Decoder& decoder, Metadata settings)
{
    std::vector<Variable> result(settings.stringSize);
    for(size_t i = 0; i < settings.stringSize; i++)
    {
        result[i] = decoder.decodeVariable(reader);
    }
    return result;
}

}