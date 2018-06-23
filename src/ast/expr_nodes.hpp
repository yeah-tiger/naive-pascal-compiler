//
// Created by yzy on 6/21/18.
//

#ifndef NAIVE_PASCAL_COMPILER_EXPR_NODES_HPP
#define NAIVE_PASCAL_COMPILER_EXPR_NODES_HPP

#include <string>
#include <map>
#include "dummy_node.hpp"
#include "identifier_node.hpp"
#include "routine_call_nodes.hpp"
#include "sys_routine_nodes.hpp"

namespace npc
{
    class ArrayRefNode : public ExprNode
    {
    public:
        std::shared_ptr<IdentifierNode> identifier;
        std::shared_ptr<ExprNode> index;

        ArrayRefNode(const NodePtr &identifier, const NodePtr &index)
                : identifier(cast_node<IdentifierNode>(identifier)), index(cast_node<ExprNode>(index))
        {}

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "ArrayRef", "identifier": )"} +
                   this->identifier->to_json() +
                   R"(, "index": )" +
                   this->index->to_json();
        }

        bool should_have_children() const override
        { return false; }
    };

    class RecordRefNode : public ExprNode
    {
    public:
        std::shared_ptr<IdentifierNode> identifier;
        std::shared_ptr<IdentifierNode> field;

        RecordRefNode(const NodePtr &identifier, const NodePtr &field)
                : identifier(cast_node<IdentifierNode>(identifier)), field(cast_node<IdentifierNode>(field))
        {}

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "RecordRef", "identifier": )"} +
                   this->identifier->to_json() +
                   R"(, "field": )" +
                   this->field->to_json();
        }

        bool should_have_children() const override
        { return false; }
    };

    enum class BinaryOperator
    {
        GT, GE, LT, LE, EQ, NE, ADD, SUB, MUL, DIV, MOD, AND, OR, XOR
    };

    inline std::string to_string(BinaryOperator binop)
    {
        std::map<BinaryOperator, std::string> binop_to_string{
                {BinaryOperator::GT,  "GT"},
                {BinaryOperator::GE,  "GE"},
                {BinaryOperator::LT,  "LT"},
                {BinaryOperator::LE,  "LE"},
                {BinaryOperator::EQ,  "EQ"},
                {BinaryOperator::NE,  "NE"},
                {BinaryOperator::ADD, "ADD"},
                {BinaryOperator::SUB, "SUB"},
                {BinaryOperator::MUL, "MUL"},
                {BinaryOperator::DIV, "DIV"},
                {BinaryOperator::MOD, "MOD"},
                {BinaryOperator::AND, "AND"},
                {BinaryOperator::OR,  "OR"},
                {BinaryOperator::XOR, "XOR"}
        };
        // TODO: bound checking
        return binop_to_string[binop];
    }

    class BinopExprNode : public ExprNode
    {
    public:
        BinaryOperator op;
        std::shared_ptr<ExprNode> lhs, rhs;

        BinopExprNode(BinaryOperator op, const NodePtr &lhs, const NodePtr &rhs)
                : op(op), lhs(cast_node<ExprNode>(lhs)), rhs(cast_node<ExprNode>(rhs))
        {}

    protected:
        bool should_have_children() const override
        { return false; }

        std::string json_head() const override
        {
            return std::string{R"("type": "BinopExpr", "op": ")"} +
                   to_string(this->op) +
                   R"(", "lhs": )" +
                   this->lhs->to_json() +
                   R"(, "rhs": )" +
                   this->rhs->to_json();
        }
    };

    class FuncExprNode : public ExprNode
    {
    public:
        NodePtr func_call;

        FuncExprNode(const NodePtr &func_call) : func_call(func_call)
        {
            assert(is_a_ptr_of<FuncCallNode>(func_call) || is_a_ptr_of<SysCallNode>(func_call));
        }

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "FuncExpr", "call": )"} + this->func_call->to_json();
        }

        bool should_have_children() const override
        { return false; }
    };
}

#endif //NAIVE_PASCAL_COMPILER_EXPR_NODES_HPP
