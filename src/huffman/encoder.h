//============================================================================
// @name        : huffmanEncoder.h
// @author      : Thomas Dooms
// @date        : 11/15/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include "huffman.h"
#include "../pal/metadata.h"

namespace huffman
{

struct Encoder
{
    Encoder(const std::vector<Variable>& string, const std::vector<Production>& productions, pal::Metadata metadata)
    {
        const auto size = metadata.settings.offset(metadata.productionSize);
        const auto freq = countFrequencies(string, productions, size);

        root  = createHuffmanTree(freq);
        table = createEncodingTable(root, size);

//        dotHuffmanTree("encode.dot", root);
    }

    void encodeVariable(Bitwriter& writer, Variable var) const
    {
        const auto [value, size] = table[var];
        writer.write_value(value, size);
    }

    std::unique_ptr<Node> root;
    EncodingTable table;
};

}

