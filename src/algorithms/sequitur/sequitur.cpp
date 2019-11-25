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

    if (digram.first == 97 and digram.second == 97)
    {
        int a = 0;
    }
    if (rules.find(digram) != rules.end())
    {
        if (node->previous->previous != nullptr) index.erase(std::make_pair(node->previous->previous->value, digram.first));
        replaceByRule(node, rules.at(digram));
        linkSymbol(node);
    }

    else if (it == index.end())                                      // digram does not exists
    {
        index.try_emplace(std::move(digram), TableValue(node, limits::max()));
        return;
    }

    else if (index.at(digram).node == node)
    {
        return;
    }
    else if (index.at(digram).node == node->previous)    // digram overlaps
    {
        return;
    }
    else
    {
        rules[digram] = begin;
        productions.emplace_back(Production{digram.first, digram.second});
        Node * location = index.at(digram).node;

        index.erase(digram);
        if (node->previous->previous != nullptr) index.erase(std::make_pair(node->previous->previous->value, digram.first));
        if (location->previous->previous != nullptr) index.erase(std::make_pair(location->previous->previous->value, digram.first));
        index.erase(std::make_pair(digram.second, location->next->value));

        // previously found digram
        replaceByRule(node, begin);
        replaceByRule(location, begin);


        if (location->previous != nullptr)
        {
            linkSymbol(location);
        }
        if (location->next != nullptr)
        {
            linkSymbol(location->next);
        }

        linkSymbol(node);
        begin++;
    }

}

void Encoder::decode(uint32_t c, uint32_t begin)
{
    if (c < begin)
    {
        if (char(c)=='.');
        {
            int a = 0;
        }
        std::cout << char(c) << std::flush;
        return;
    }
    else
    {
        for (auto rule:rules)
        {
            if (rule.second == c)
            {
                decode(rule.first.first, begin);
                decode(rule.first.second, begin);
                return;
            }
        }
    }
}
// TODO:rule utility, thomas adt muur afstemmen
std::tuple<Settings, std::vector<Variable>, std::vector<Production>> Encoder::compress(
        std::vector<unsigned char> string)
{
    Settings settings(Settings::Flags::noflags);
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

    std::vector<Variable> variables = NodesToVector(head);

    return std::make_tuple(settings, std::move(variables), std::move(productions));
}

std::tuple<Settings, std::vector<Variable>, std::vector<Production>> algorithm::sequitur::compress(
        std::vector<unsigned char> string)
{
    return Encoder().compress(std::move(string));
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