//============================================================================
// @name        : huffmanEncoder.h
// @author      : Thomas Dooms
// @date        : 11/15/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <memory>
#include "huffman.h"

struct HuffmanEncoder
{
    HuffmanEncoder(const std::vector<uint32_t>& string, const std::vector<Production>& productions, Settings settings)
    {
        const auto size = settings.start() + settings.productionSize;
        const auto freq = Huffman::count_frequencies(string, productions, size);

        root  = Huffman::create_huffman_tree(freq);
        table = Huffman::create_huffman_encode_table(root, size);

//        Huffman::dot_huffman_tree("encode.dot", root);
    }

    void encode_character(Bitwriter& writer, uint32_t character) const
    {
        const auto [value, size] = table[character];
        writer.write_value(value, size);
    }

    std::unique_ptr<Huffman::Node> root;
    Huffman::EncodingTable table;
};

