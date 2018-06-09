//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_PROCEDURE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_PROCEDURE_NODES_HPP

#include "dummy_node.hpp"
#include "parameter_nodes.hpp"
#include "type_nodes.hpp"

namespace npc
{
    class RoutineNode : public DummyNode
    {
    };

    class SubRoutineNode : public DummyNode
    {
    };

    class ProcedureNode : public DummyNode
    {
    };

    class FunctionHeadNode : public DummyNode
    {
    public:
        std::shared_ptr<ParametersNode> parameters;
        std::shared_ptr<TypeDeclNode> type;

        FunctionHeadNode(const NodePtr &params, const NodePtr &_type)
                : parameters(cast_node<ParametersNode>(params)),
                  type(cast_node<TypeDeclNode>(_type))
        {}
    };

    class FunctionNode : public DummyNode
    {
    public:
        std::shared_ptr<FunctionHeadNode> head;
    };
}

#endif //NAIVE_PASCAL_COMPILER_PROCEDURE_NODES_HPP
