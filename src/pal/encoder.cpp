//============================================================================
// @name        : palEncoder.cpp
// @author      : Thomas Dooms
// @date        : 11/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================

#include "encoder.h"

namespace pal
{

void Encoder::encode(const std::filesystem::path& path, const std::vector<Variable>& string, const std::vector<Production>& productions, Metadata settings)
{
    huffman::Encoder encoder(string, productions, settings);
    Bitwriter writer(path);

    encodeMetadata   (writer, settings);
    encodeHuffmanTree(writer, encoder, settings);
    encodeProductions(writer, encoder, productions);
    encodeString     (writer, encoder, string);
}

void Encoder::encodeMetadata(Bitwriter& writer, Metadata settings)
{
    writer.write_value(settings.stringSize);
    writer.write_value(settings.productionSize);
    writer.write_value(settings.flags);
}

void Encoder::encodeHuffmanTree(Bitwriter& writer, const huffman::Encoder& encoder, Metadata settings)
{
    const std::function<void(Bitwriter&, const huffman::Node*, uint32_t)> recursion = [&](auto& writer, const auto* root, auto charLength) -> void
    {
        if(root->leaf())
        {
            writer.write_bit(true);
            writer.write_value(root->character, charLength);
        }
        else
        {
            writer.write_bit(false);
            recursion(writer, root->left.get() , charLength);
            recursion(writer, root->right.get(), charLength);
        }
    };

    recursion(writer, encoder.root.get(), settings.charLength);
}

void Encoder::encodeProductions(Bitwriter& writer, const huffman::Encoder& encoder, const std::vector<Production>& productions)
{
    for(const auto production : productions)
    {
        encoder.encodeVariable(writer, production.body[0]);
        encoder.encodeVariable(writer, production.body[1]);
    }
}

void Encoder::encodeString(Bitwriter& writer, const huffman::Encoder& encoder, const std::vector<Variable>& string)
{
    for(const auto variable : string)
    {
        encoder.encodeVariable(writer, variable);
    }
}

}