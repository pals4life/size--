//============================================================================
// @name        : huffman.h
// @author      : Thomas Dooms
// @date        : 11/12/19
// @version     : 
// @copyright   : BA1 Informatica - Thomas Dooms - University of Antwerp
// @description : 
//============================================================================


#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>


#include "../bitwriter.h"
#include "../bitreader.h"
#include "../settings.h"



struct Production
{
    uint32_t head;
    std::array<uint32_t, 2> body;
};

struct Huffman
{
    struct Node
    {
        Node()                       : character(0), frequency(0), left(nullptr), right(nullptr) {}
        Node(uint32_t c)             : character(c), frequency(0), left(nullptr), right(nullptr) {}
        Node(uint32_t c, uint32_t f) : character(c), frequency(f), left(nullptr), right(nullptr) {}

        bool leaf(){ return character != std::numeric_limits<uint32_t>::max(); }

        uint32_t character;
        uint32_t frequency;

        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };


    using EncodingTable = std::vector<std::pair<uint32_t, uint8_t>>;

    [[nodiscard]] static std::vector<uint32_t> count_frequencies(const std::vector<uint32_t>& string, const std::vector<Production>& productions, uint32_t size);
    [[nodiscard]] static std::unique_ptr<Node> create_huffman_tree(const std::vector<uint32_t>& frequencies);
    [[nodiscard]] static EncodingTable create_huffman_encode_table(const std::unique_ptr<Node>& root, uint32_t num_chars);

    static void print_encoding_table(const EncodingTable& table);
};





















