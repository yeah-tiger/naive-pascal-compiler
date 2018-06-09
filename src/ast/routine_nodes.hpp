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

    class ProcedureHeadNode : public DummyNode
    {
    public:
        std::shared_ptr<ParametersNode> parameters;

        ProcedureHeadNode(const NodePtr &params)
                : parameters(cast_node<ParametersNode>(params))
        {}
    };

    class ProcedureNode : public DummyNode
    {
    public:
        std::shared_ptr<ProcedureHeadNode> head;

        ProcedureNode(const NodePtr &_head, const NodePtr &_body)
                : head(cast_node<ProcedureHeadNode>(_head))
        {
            this->children() = _body->children();
        }
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

        FunctionNode(const NodePtr &_head, const NodePtr &_body)
                : head(cast_node<FunctionHeadNode>(_head))
        {
            this->children() = _body->children();
        }
    };
}

#endif //NAIVE_PASCAL_COMPILER_PROCEDURE_NODES_HPP
