//
// Created by yzy on 6/21/18.
//

#ifndef NAIVE_PASCAL_COMPILER_EXPR_NODES_HPP
#define NAIVE_PASCAL_COMPILER_EXPR_NODES_HPP

#include <string>
#include <map>
#include "dummy_node.hpp"
#include "type_nodes.hpp"
#include "const_value_nodes.hpp"
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

        virtual Type get_type() const {
            Type lhs_type, rhs_type;
            lhs_type = lhs->get_type();
            rhs_type = rhs->get_type();
            if (lhs_type != rhs_type &&
                !((lhs_type == Type::INTEGER && rhs_type == Type::REAL)
                        || (lhs_type == Type::REAL && rhs_type == Type::INTEGER)
                )) {
                std::cerr << "Type mismatch: " << to_string(lhs_type) << ", " << to_string(rhs_type) << std::endl;
                assert(lhs_type == rhs_type);
            }
            return lhs_type;
        }

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

        virtual Type get_type() const {
            if (is_a_ptr_of<FuncCallNode>(func_call)) {
                auto type = cast_node<FuncCallNode>(func_call)->identifier->get_type();
                return type;
            } else {
                auto routine = cast_node<SysCallNode>(func_call)->routine->routine;
                // TODO: IMPLEMENT REAL SYS CALL TYPE DECL.
                return Type::REAL;
            }
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
