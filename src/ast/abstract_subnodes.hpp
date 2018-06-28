//
// Created by 孙耀珠 on 2018/6/28.
//

#ifndef NAIVE_PASCAL_COMPILER_ABSTRACT_SUBNODES_HPP
#define NAIVE_PASCAL_COMPILER_ABSTRACT_SUBNODES_HPP

#include "ast/dummy_node.hpp"

namespace npc
{
    class ExprNode : public DummyNode
    {
    protected:
        ExprNode() = default;
    };

    class StmtNode : public DummyNode
    {
    protected:
        StmtNode() = default;
    };
}

#endif //NAIVE_PASCAL_COMPILER_ABSTRACT_SUBNODES_HPP
