// =====================================================================
// @name: TableValue.h
// @project: SIZE--
// @author: Mano Marichal
// @date: 23.11.19
// @copyright: BA2 Informatica - Mano Marichal - University of Antwerp
// @description: 
// =====================================================================

#ifndef SIZE_TABLEVALUE_H
#define SIZE_TABLEVALUE_H

#include <stdint-gcc.h>
#include "./Node.h"
namespace algorithm::sequitur
{
    struct TableValue
    {
        Node * node;
        uint32_t rule;
        TableValue(Node* node, uint32_t rule): node(node), rule(rule) {};
    };
}
#endif //SIZE_TABLEVALUE_H
