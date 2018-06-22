//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_STMT_NODES_HPP
#define NAIVE_PASCAL_COMPILER_STMT_NODES_HPP

#include <utility>
#include "dummy_node.hpp"
#include "type_nodes.hpp"
#include "const_value_nodes.hpp"
#include "routine_call_nodes.hpp"

namespace npc
{

    class StmtNode : public DummyNode
    {
    };

    class CompoundStmtNode : public StmtNode
    {
    };


    class AssignStmtNode : public StmtNode
    {
    public:
        std::shared_ptr<ExprNode> lhs;
        std::shared_ptr<ExprNode> rhs;

        AssignStmtNode(const NodePtr &lhs, const NodePtr &rhs)
                : lhs(cast_node<ExprNode>(lhs)), rhs(cast_node<ExprNode>(rhs))
        {
            assert(is_a_ptr_of<IdentifierNode>(lhs)
                || is_a_ptr_of<ArrayRefNode>(lhs) || is_a_ptr_of<RecordRefNode>(lhs));
        }

    };

    class ProcStmtNode : public StmtNode
    {
    public:
        NodePtr proc_call;

        ProcStmtNode(const NodePtr &proc_call) : proc_call(proc_call)
        {
            assert(is_a_ptr_of<ProcCallNode>(proc_call) || is_a_ptr_of<SysCallNode>(proc_call));
        }
    };

    class IfStmtNode : public StmtNode
    {
    public:
        std::shared_ptr<ExprNode> expr;
        std::shared_ptr<StmtNode> stmt;
        std::shared_ptr<StmtNode> else_stmt;

        IfStmtNode(const NodePtr &expr, const NodePtr &stmt, const NodePtr &else_stmt)
                : expr(cast_node<ExprNode>(expr)), stmt(cast_node<StmtNode>(stmt)),
                  else_stmt(cast_node<StmtNode>(else_stmt))
        {}
    };

    class RepeatStmtNode : public StmtNode
    {
    public:
        std::shared_ptr<ExprNode> expr;

        RepeatStmtNode(const NodePtr &expr) : expr(cast_node<ExprNode>(expr)) {}
    };

    class WhileStmtNode : public StmtNode
    {
    public:
        std::shared_ptr<ExprNode> expr;
        std::shared_ptr<StmtNode> stmt;

        WhileStmtNode(const NodePtr &expr, const NodePtr &stmt)
                : expr(cast_node<ExprNode>(expr)), stmt(cast_node<StmtNode>(stmt))
        {}
    };

    enum class DirectionEnum
    {
        TO, DOWNTO
    };

    class ForStmtNode : public StmtNode
    {
    public:
        DirectionEnum direction;
        std::shared_ptr<IdentifierNode> identifier;
        std::shared_ptr<ExprNode> start;
        std::shared_ptr<ExprNode> finish;
        std::shared_ptr<StmtNode> stmt;

        ForStmtNode(DirectionEnum direction, const NodePtr &identifier,
                    const NodePtr &start, const NodePtr &finish, const NodePtr &stmt)
                : direction(direction), identifier(cast_node<IdentifierNode>(identifier)),
                  start(cast_node<ExprNode>(start)), finish(cast_node<ExprNode>(finish)),
                  stmt(cast_node<StmtNode>(stmt))
        {}
    };

    class CaseExprNode : public StmtNode
    {
    public:
        std::shared_ptr<ExprNode> branch;
        std::shared_ptr<StmtNode> stmt;

        CaseExprNode(const NodePtr &branch, const NodePtr &stmt)
                : branch(cast_node<ExprNode>(branch)), stmt(cast_node<StmtNode>(stmt))
        {
            assert(is_a_ptr_of<IdentifierNode>(branch) || is_a_ptr_of<ConstValueNode>(branch));
        }
    };

    class CaseStmtNode : public StmtNode
    {
    public:
        std::shared_ptr<ExprNode> expr;

        void add_expr(const NodePtr &expr)
        {
            this->expr = cast_node<ExprNode>(expr);
        }
    };

    class StmtList : public StmtNode
    {
    };
}

#endif //NAIVE_PASCAL_COMPILER_STMT_NODES_HPP
