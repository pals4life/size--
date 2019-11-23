// =====================================================================
// @name: sequitur.h
// @project: SIZE--
// @author: Mano Marichal
// @date: 18.11.19
// @copyright: BA2 Informatica - Mano Marichal - University of Antwerp
// @description: 
// =====================================================================

#pragma once

#include <vector>
#include <algorithm>
#include <numeric>
#include <list>
#include <fstream>
#include <unordered_map>
#include <boost/functional/hash.hpp>

#include "../../util/variable.h"
#include "../../util/production.h"
#include "../../util/settings.h"
#include "./Node.h"
#include "./TableValue.h"
#include <tuple>

using namespace algorithm::sequitur;

namespace algorithm::sequitur
{
    using Digram = std::pair<uint32_t, uint32_t>;
    using limits = std::numeric_limits<uint32_t>;

    class Encoder
    {
        std::vector<Node> nodes;
        std::unordered_map<Digram, TableValue, boost::hash<Digram>> index;
        uint32_t begin;
        Node* head;
        void erase(Node * node);
        void linkSymbol(Node* &node);
        void replaceByRule(Node* node, uint32_t rule);
        static std::vector<Node> vectorToNodes(const std::vector<unsigned char> &input);
        static std::vector<uint32_t> NodesToVector(Node * start);
        //void decode(const std::unordered_map<Digram, uint32_t, boost::hash<Digram>> &rules, u_int32_t c, u_int32_t begin);

    public:
        std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(std::vector<unsigned char> string);

    };

    std::tuple<Settings, std::vector<Variable>, std::vector<Production>> compress(std::vector<unsigned char> string);

    // std::pair<std::vector<Variable>, std::vector<Production>>
}
