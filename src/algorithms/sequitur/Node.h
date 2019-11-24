// =====================================================================
// @name: Node.h
// @project: SIZE--
// @author: Mano Marichal
// @date: 23.11.19
// @copyright: BA2 Informatica - Mano Marichal - University of Antwerp
// @description: 
// =====================================================================

#ifndef SIZE_NODE_H
#define SIZE_NODE_H
namespace algorithm::sequitur
{
    struct Node
    {
        uint32_t value;
        Node* next = nullptr;
        Node* previous = nullptr;
        Node(uint32_t value, Node* next, Node* previous): value(value), next(next), previous(previous) {};
        Node(uint32_t value): value(value) {};
    };
}
#endif //SIZE_NODE_H
