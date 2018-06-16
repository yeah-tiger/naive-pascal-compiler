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
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<ParametersNode> parameters;

        ProcedureHeadNode(const NodePtr &_name, const NodePtr &params)
                : parameters(cast_node<ParametersNode>(params)), name(cast_node<IdentifierNode>(_name))
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
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<ParametersNode> parameters;
        std::shared_ptr<TypeDeclNode> type;

        FunctionHeadNode(const NodePtr &_name, const NodePtr &params, const NodePtr &_type)
                : parameters(cast_node<ParametersNode>(params)),
                  type(cast_node<TypeDeclNode>(_type)), name(cast_node<IdentifierNode>(_name))
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

    class ProcCallNode : public DummyNode
    {
    public:
        ProcCallNode(NodePtr &_ID)
                : ID(cast_node<IdentifierNode>(ID))
        {}

        ProcCallNode(NodePtr &_ID, NodePtr &_args)
                : ID(cast_node<IdentifierNode>(ID)), args(cast_node<ExprListNode>(_args))
        {}

        std::shared_ptr<IdentifierNode> ID;
        std::shared_ptr<ExprListNode> args;
    };

    class SysProcCallNode : public DummyNode
    {
    public:
        SysProcCallNode(NodePtr &_ID)
                : ID(cast_node<IdentifierNode>(ID))
        {}

        SysProcCallNode(NodePtr &_ID, NodePtr &_args)
                : ID(cast_node<IdentifierNode>(ID)), args(cast_node<ExprListNode>(_args))
        {}

        std::shared_ptr<IdentifierNode> ID;
        std::shared_ptr<ExprListNode> args;
    };
}

#endif //NAIVE_PASCAL_COMPILER_PROCEDURE_NODES_HPP
