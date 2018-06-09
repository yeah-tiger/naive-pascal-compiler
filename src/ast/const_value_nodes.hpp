//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_CONST_VALUE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_CONST_VALUE_NODES_HPP

#include "dummy_node.hpp"

namespace npc
{
    class ConstValueNode : public DummyNode
    {
    };

    class IntegerNode : public ConstValueNode
    {
    };

    class RealNode : public ConstValueNode
    {
    };

    class CharNode : public ConstValueNode
    {
    };

    class StringNode : public ConstValueNode
    {
    };

    enum class SysConEnum
    {
        TRUE,
        FALSE,
        MAXINT
    };

    class SysConNode : public ConstValueNode
    {
    public:
        SysConEnum val;

        SysConNode(SysConEnum v)
                : val(v)
        {}
    };
}

#endif //NAIVE_PASCAL_COMPILER_CONST_VALUE_NODES_HPP
