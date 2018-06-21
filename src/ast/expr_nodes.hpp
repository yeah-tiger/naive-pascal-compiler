//
// Created by yzy on 6/21/18.
//

#ifndef NAIVE_PASCAL_COMPILER_EXPR_NODES_HPP
#define NAIVE_PASCAL_COMPILER_EXPR_NODES_HPP

#include "abstract_expr_node.hpp"

namespace npc
{

    class ArrayRefNode : public AbstractExprNode
    {
    public:
        ///
        /// \param Id
        /// \param index AbstractExprNode or IdentifierNode
        ArrayRefNode(const NodePtr &Id, const NodePtr &index)
                : ID(cast_node<IdentifierNode>(Id)), index(index)
        {}

        std::shared_ptr<IdentifierNode> ID;
        std::shared_ptr<AbstractNode> index;
    };

    class BinopExprNode : public AbstractExprNode
    {
    public:
        enum class OP
        {
            gt, ge, lt, le, eq, neq, plus, minus, mult, div, mod, _and, _or, _xor
        };
        OP op;
        NodePtr lhs, rhs;

        BinopExprNode(OP _Op, NodePtr _lhs, NodePtr _rhs)
                : op(_Op), lhs(std::move(_lhs)),
                  rhs(std::move(_rhs))
        {}
    };

    class ExprListNode : public AbstractExprNode
    {
    };
}

#endif //NAIVE_PASCAL_COMPILER_EXPR_NODES_HPP
