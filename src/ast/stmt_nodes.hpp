//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_STMT_NODES_HPP
#define NAIVE_PASCAL_COMPILER_STMT_NODES_HPP

#include "ast/abstract_subnodes.hpp"
#include "ast/const_value_nodes.hpp"
#include "ast/routine_call_nodes.hpp"
#include "ast/sys_routine_nodes.hpp"

namespace npc
{
    class CompoundStmtNode : public StmtNode
    {
    public:
        llvm::Value *codegen(CodegenContext &context) override;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "CompoundStmt")"};
        }

        bool should_have_children() const override
        { return true; }
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

        llvm::Value *codegen(CodegenContext &context) override;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "AssignStmt", "lhs": )"} +
                   this->lhs->to_json() +
                   R"(, "rhs": )" +
                   this->rhs->to_json();
        }

        bool should_have_children() const override
        { return false; }
    };

    class ProcStmtNode : public StmtNode
    {
    public:
        NodePtr proc_call;

        ProcStmtNode(const NodePtr &proc_call) : proc_call(proc_call)
        {
            assert(is_a_ptr_of<RoutineCallNode>(proc_call) || is_a_ptr_of<SysCallNode>(proc_call));
        }

        llvm::Value *codegen(CodegenContext &context) override;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "ProcStmt", "call": )"} +
                   this->proc_call->to_json();
        }

        bool should_have_children() const override
        { return false; }
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

        llvm::Value *codegen(CodegenContext &context) override;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "IfStmt", "expr": )"} +
                   this->expr->to_json() +
                   R"(, "stmt": )" +
                   this->stmt->to_json() +
                   R"(, "else_stmt": )" +
                   this->else_stmt->to_json();
        }

        bool should_have_children() const override
        { return false; }
    };

    class RepeatStmtNode : public StmtNode
    {
    public:
        std::shared_ptr<ExprNode> expr;

        RepeatStmtNode(const NodePtr &expr) : expr(cast_node<ExprNode>(expr))
        {}

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "RepeatStmt", "expr": )"} +
                   this->expr->to_json();
        }

        bool should_have_children() const override
        { return true; }
    };

    class WhileStmtNode : public StmtNode
    {
    public:
        std::shared_ptr<ExprNode> expr;
        std::shared_ptr<StmtNode> stmt;

        WhileStmtNode(const NodePtr &expr, const NodePtr &stmt)
                : expr(cast_node<ExprNode>(expr)), stmt(cast_node<StmtNode>(stmt))
        {}

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "WhileStmt", "expr": )"} +
                   this->expr->to_json() +
                   R"(, "stmt": )" +
                   this->stmt->to_json();
        }

        bool should_have_children() const override
        { return false; }
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

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "ForStmt", "direction": ")"} +
                   (direction == DirectionEnum::TO ? "TO" : "DOWNTO") +
                   R"(", "identifier": )" + this->identifier->to_json() +
                   R"(, "start": )" + this->start->to_json() +
                   R"(, "finish": )" + this->finish->to_json() +
                   R"(, "stmt": )" + this->stmt->to_json();
        }

        bool should_have_children() const override
        { return false; }
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

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "CaseExpr", "branch": )"} +
                   this->branch->to_json() +
                   R"(, "stmt": )" +
                   this->stmt->to_json();
        }

        bool should_have_children() const override
        { return false; }
    };

    class CaseStmtNode : public StmtNode
    {
    public:
        std::shared_ptr<ExprNode> expr;

        void add_expr(const NodePtr &expr)
        {
            this->expr = cast_node<ExprNode>(expr);
        }

        llvm::Value *codegen(CodegenContext &context) override;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "CaseStmt", "expr": )"} +
                   this->expr->to_json();
        }

        bool should_have_children() const override
        { return true; }
    };

    // Intermediate container, would be removed
    class StmtList : public StmtNode
    {
    };
}

#endif //NAIVE_PASCAL_COMPILER_STMT_NODES_HPP
