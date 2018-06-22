//
// Created by 孙耀珠 on 2018/6/22.
//

#ifndef NAIVE_PASCAL_COMPILER_HEAD_LIST_NODES_HPP
#define NAIVE_PASCAL_COMPILER_HEAD_LIST_NODES_HPP

#include "dummy_node.hpp"
#include "const_expr_node.hpp"

namespace npc
{
    class ConstListNode : public DummyNode
    {
    };

    class TypeListNode : public DummyNode
    {
    };

    class VarListNode : public DummyNode
    {
    };

    class SubroutineListNode : public DummyNode
    {
    };

    class HeadListNode : public DummyNode
    {
    public:
        std::shared_ptr<ConstListNode> const_list;
        std::shared_ptr<TypeListNode> type_list;
        std::shared_ptr<VarListNode> var_list;
        std::shared_ptr<SubroutineListNode> subroutine_list;
    };
}

#endif //NAIVE_PASCAL_COMPILER_HEAD_LIST_NODES_HPP
