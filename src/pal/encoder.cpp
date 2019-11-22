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
    encodeProductions(writer, encoder, productions, metadata);
    encodeString     (writer, encoder, string);
}

void Encoder::encodeMetadata(Bitwriter& writer, Metadata metadata)
{
    writer.write_value(metadata.stringSize);
    writer.write_value(metadata.productionSize);
    writer.write_value(metadata.settings.flags);
    writer.write_value(static_cast<uint8_t>(93u));
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

void Encoder::encodeProductions(Bitwriter& writer, const huffman::Encoder& encoder, const std::vector<Production>& productions, Metadata metadata)
{
    for(size_t i = 0; i < productions.size(); i++)
    {
        // if the production is not used, write a 1, else write a 0
        if(metadata.settings.has_smart_productions() )
        {
            if(encoder.freq[metadata.settings.offset(i)] == 0)
            {
                writer.write_bit(true);
                continue;
            }
            else writer.write_bit(false);
        }
        encoder.encodeVariable(writer, productions[i][0]);
        encoder.encodeVariable(writer, productions[i][1]);
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