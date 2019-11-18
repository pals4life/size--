//============================================================================
// @name        : palEncoder.h
// @author      : Thomas Dooms
// @date        : 11/16/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include "../bitio/bitwriter.h"

#include "../huffman/encoder.h"

namespace pal
{
    struct Encoder
    {
        static void encode(const std::filesystem::path& path, const std::vector<Variable>& string, const std::vector<Production>& productions, Metadata settings);

        static void encodeMetadata   (Bitwriter& writer, Metadata settings);
        static void encodeHuffmanTree(Bitwriter& writer, const huffman::Encoder& encoder, Metadata settings);
        static void encodeProductions(Bitwriter& writer, const huffman::Encoder& encoder, const std::vector<Production>& productions);
        static void encodeString     (Bitwriter& writer, const huffman::Encoder& encoder, const std::vector<Variable>& string);
    };
}




