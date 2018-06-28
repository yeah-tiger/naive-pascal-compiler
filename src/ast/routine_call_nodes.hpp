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

        llvm::Value *codegen(CodegenContext &context) override;

    protected:
        RoutineCallNode() = default;

        bool should_have_children() const final
        { return false; }
    };

    class ProcCallNode : public RoutineCallNode
    {
    public:
        using RoutineCallNode::RoutineCallNode;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "ProcCall", "identifier": )"} + this->identifier->to_json() +
                   R"(, "args": )" + this->args->to_json();
        }
    };

    class FuncCallNode : public RoutineCallNode
    {
    public:
        using RoutineCallNode::RoutineCallNode;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "FuncCall", "identifier": )"} + this->identifier->to_json() +
                   R"(, "args": )" + this->args->to_json();
        }
    };
}

#endif //NAIVE_PASCAL_COMPILER_ROUTINE_CALL_NODES_HPP
