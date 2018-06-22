//
// Created by 孙耀珠 on 2018/6/22.
//

#ifndef NAIVE_PASCAL_COMPILER_ROUTINE_CALL_NODES_HPP
#define NAIVE_PASCAL_COMPILER_ROUTINE_CALL_NODES_HPP

#include "dummy_node.hpp"
#include "type_nodes.hpp"
#include "sys_routine_nodes.hpp"

namespace npc
{
    class ProcCallNode : public DummyNode
    {
    public:
        explicit ProcCallNode(const NodePtr &_ID)
                : ID(cast_node<IdentifierNode>(_ID))
        {}

        ProcCallNode(const NodePtr &_ID, const NodePtr &_args)
                : ID(cast_node<IdentifierNode>(_ID)), args(cast_node<ExprListNode>(_args))
        {}

        std::shared_ptr<IdentifierNode> ID;
        std::shared_ptr<ExprListNode> args;
    };

    class SysProcCallNode : public DummyNode
    {
    public:
        explicit SysProcCallNode(const NodePtr &_ID)
                : ID(cast_node<SysFunctionNode>(_ID))
        {}

        SysProcCallNode(const NodePtr &_ID, const NodePtr &_args)
                : ID(cast_node<SysFunctionNode>(_ID)), args(_args)
        {}

        std::shared_ptr<SysFunctionNode> ID;
        std::shared_ptr<AbstractNode> args;  // ExprListNode or FActor
    };
}

#endif //NAIVE_PASCAL_COMPILER_ROUTINE_CALL_NODES_HPP
