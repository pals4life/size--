// =====================================================================
// @name: sequitur.cpp
// @project: SIZE--
// @author: Mano Marichal
// @date: 18.11.19
// @copyright: BA2 Informatica - Mano Marichal - University of Antwerp
// @description: 
// =====================================================================

#include "sequitur.h"
#include <iostream>
#include <fstream>
using namespace algorithm::sequitur;

void Encoder::erase(Node *node)
{
    if (node->previous != nullptr)
    {
        node->previous->next = node->next;
    } else head = node->next;
    if (node->next != nullptr) node->next->previous = node->previous;
}

std::vector<Node> Encoder::vectorToNodes(const std::vector<unsigned char> &input)
{
    std::vector<Node> nodes;
    nodes.reserve(input.size());
    for (auto var:input)
    {
        nodes.emplace_back(Node(var));
    }

    for (int i=0;i<nodes.size();i++)
    {
        if (i != 0) nodes[i].previous = &nodes[i-1];
        if (i != nodes.size()-1) nodes[i].next = &nodes[i+1];
    }
    return std::move(nodes);
}

std::vector<uint32_t> Encoder::NodesToVector(Node *start)
{
    std::vector<uint32_t> variables;
    bool check = false;
    while (start != nullptr)
    {
        variables.emplace_back(start->value);
        start = start->next;
    }
    return std::move(variables);
}

void Encoder::replaceByRule(Node *node, uint32_t rule)
{
    node->value = rule;
    erase(node->previous);
}
void Encoder::linkSymbol(Node* &node)
{
    Digram digram = {node->previous->value, node->value};
    auto it = index.find(digram);

    if (it == index.end())                                      // digram does not exists
    {
        index.try_emplace(std::move(digram), TableValue(node, limits::max()));
        return;
    }

    else if (node->previous->previous->value == node->previous->value
            and node->value == node->previous->value)    // digram overlaps
    {
        return;
    }

    if (it->second.rule != limits::max())                            // there exists a complete rule for this digram
    {
        replaceByRule(node, it->second.rule);
    }
    else
    {

        it->second.rule = begin;                                // make rule
        replaceByRule(node, begin);                         // replace current digram
        replaceByRule(index.at(digram).node, begin);  // replace previously found digram

        begin++;

    }
}

// TODO:rule utility, thomas adt muur afstemmen
std::tuple<Settings, std::vector<Variable>, std::vector<Production>> Encoder::compress(
        std::vector<unsigned char> string)
{
    Settings settings(Settings::Flags::noflags);
    std::vector<Production> productions;
    productions.reserve(string.size());

    begin = settings.begin();

    nodes.reserve(string.size());
    for (auto var:string)
    {
        nodes.emplace_back(Node(var));
    }

    for (int i=0;i<nodes.size();i++)
    {
        if (i != 0) nodes[i].previous = &nodes[i-1];
        if (i != nodes.size()-1) nodes[i].next = &nodes[i+1];
    }

    head = &nodes[0];
    Node* node = &nodes[1];     // we start from the second symbol
    while (node != nullptr)
    {
        linkSymbol(node);
        node = node->next;
    }

    std::vector<uint32_t> test = NodesToVector(head);

    return {settings.begin(), {}, {}};
}

std::tuple<Settings, std::vector<Variable>, std::vector<Production>> algorithm::sequitur::compress(
        std::vector<unsigned char> string)
{
    return Encoder().compress(string);
}
/*
 *

void algorithm::sequitur::decode(const std::unordered_map<Digram, uint32_t, boost::hash<Digram>> &rules, u_int32_t c, u_int32_t begin)
{
    if (c == std::numeric_limits<uint32_t>::max()) return;

    if (c < begin)
    {
        std::cout << char(c) << std::flush;
        return;
    }

    for (auto &rule:rules)
    {
        if (rule.second == c)
        {
            decode(rules, rule.first.first, begin);
            decode(rules, rule.first.second, begin);
            return;
        }
    }
}
 */