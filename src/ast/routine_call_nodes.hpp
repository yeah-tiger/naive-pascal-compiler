//
// Created by 孙耀珠 on 2018/6/22.
//

#ifndef NAIVE_PASCAL_COMPILER_ROUTINE_CALL_NODES_HPP
#define NAIVE_PASCAL_COMPILER_ROUTINE_CALL_NODES_HPP

#include "dummy_node.hpp"
#include "identifier_node.hpp"
#include "expr_nodes.hpp"

namespace npc
{
    class RoutineCallNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> identifier;
        std::shared_ptr<ExprListNode> args;

        RoutineCallNode(const NodePtr &identifier, const NodePtr &args)
                : identifier(cast_node<IdentifierNode>(identifier)), args(cast_node<ExprListNode>(args))
        {}

        explicit RoutineCallNode(const NodePtr &identifier)
                : RoutineCallNode(identifier, make_node<ExprListNode>())
        {}
    };

    class ProcCallNode : public RoutineCallNode
    {
    public:
        using RoutineCallNode::RoutineCallNode;
    };

    class FuncCallNode : public RoutineCallNode
    {
    public:
        using RoutineCallNode::RoutineCallNode;
    };
}

#endif //NAIVE_PASCAL_COMPILER_ROUTINE_CALL_NODES_HPP
