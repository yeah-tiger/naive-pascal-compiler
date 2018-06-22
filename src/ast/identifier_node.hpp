//
// Created by 孙耀珠 on 2018/6/22.
//

#ifndef NAIVE_PASCAL_COMPILER_IDENTIFIER_NODE_HPP
#define NAIVE_PASCAL_COMPILER_IDENTIFIER_NODE_HPP

#include "dummy_node.hpp"
#include "expr_nodes.hpp"

namespace npc
{
    class IdentifierNode : public ExprNode
    {
    public:
        std::string name;

        explicit IdentifierNode(std::string s) : name(std::move(s)) {}
        explicit IdentifierNode(const char *c) : name(c) {}
    };
}

#endif //NAIVE_PASCAL_COMPILER_IDENTIFIER_NODE_HPP
