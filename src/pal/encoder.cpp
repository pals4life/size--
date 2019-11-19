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

void Encoder::encode(const std::string& path, const std::vector<Variable>& string, const std::vector<Production>& productions, Metadata metadata)
{
    huffman::Encoder encoder(string, productions, metadata);
    Bitwriter writer(path);

    encodeMetadata   (writer, metadata);
    encodeHuffmanTree(writer, encoder, metadata);
    encodeProductions(writer, encoder, productions);
    encodeString     (writer, encoder, string);
}

void Encoder::encodeMetadata(Bitwriter& writer, Metadata metadata)
{
    writer.write_value(metadata.stringSize);
    writer.write_value(metadata.productionSize);
    writer.write_value(metadata.settings.flags);
}

void Encoder::encodeHuffmanTree(Bitwriter& writer, const huffman::Encoder& encoder, Metadata metadata)
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

    recursion(writer, encoder.root.get(), metadata.charLength);
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