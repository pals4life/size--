//============================================================================
// @name        : pal.h
// @author      : Thomas Dooms
// @date        : 11/14/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <filesystem>
#include <vector>

#include "huffman/huffmanEncoder.h"
#include "huffman/huffmanDecoder.h"

#include "settings.h"


/*
 * elk character is 32 bit, misschien maak ik een using type aan.
 *
 * het is de bedoeling dat je een vector meegeeft van characters
 *
 * de eerste 256 zijn gereserveerd voor de gewone byte mogelijkheden
 * de volgende 65536 zijn ook gereserveerd voor elke mogelijke combinatie van de basis bits.
 * door dit te doen zullen de meest gebruikte transities al gehardcode zijn in het systeem.
 *
 * ik ga een helper klasse maken die alle nieuwe character waarden kiezen vermakkelijkt.
 * het komt erop neer dat die gaat checken of de productie "hardcoded" is, dan returned die een waarde
 * andere begint die vanaf 65636 + 256 waarden te geven aan de nieuwe producties.
 *
 * de productie vector moet dus alleen de niet "hardcoded" variabelen hebben.
 *
 * huffman gaat alle waarden die gereserveerd zijn maar niet gebruikt weg optimiseren
 * zodat deze geen extra geheugen in beslag nemen
 */

namespace pal
{
    void encode(const std::filesystem::path& input, const std::vector<uint32_t>& string, const std::vector<Production>& productions);
    void decode(const std::filesystem::path& input, const std::filesystem::path& output);

    void encodeSettings(Bitwriter& writer, Settings settings);
    void encodeHuffmanTree(Bitwriter& writer, const std::unique_ptr<Huffman::Node>& root, Settings settings);
    void encodeProductions(Bitwriter& writer, HuffmanEncoder& encoder, const std::vector<Production>& productions);
    void encodeString(Bitwriter& writer, const HuffmanEncoder& encoder, const std::vector<uint32_t>& string);

    Settings decodeSettings(Bitreader& reader);
    std::unique_ptr<Huffman::Node> decodeHuffmanTree(Bitreader& reader, Settings settings);
    std::vector<Production> decodeProductions(Bitreader& reader, HuffmanDecoder& decoder, Settings settings);
    std::vector<uint32_t> decodeString(Bitreader& reader, HuffmanDecoder& decoder, Settings settings);

    std::string calculateYield(const std::vector<uint32_t>& string, const std::vector<Production>& productions, Settings settings);
}
