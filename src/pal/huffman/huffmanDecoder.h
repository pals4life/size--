//============================================================================
// @name        : huffmanDecoder.h
// @author      : Thomas Dooms
// @date        : 11/15/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <memory>
#include "huffman.h"

struct HuffmanDecoder
{
    explicit HuffmanDecoder(std::unique_ptr<Huffman::Node> root) : root(std::move(root))
    {
//        Huffman::dot_huffman_tree("decode.dot", this->root);
    }

    uint32_t decode_character(Bitreader& reader) const
    {
        Huffman::Node* node = root.get();
        while(not node->leaf())
        {
            // go left if 1, go right if 0
            node = reader.read_bit() ? node->left.get() : node->right.get();
        }
        return node->character;
    }

    std::unique_ptr<Huffman::Node> root;
};