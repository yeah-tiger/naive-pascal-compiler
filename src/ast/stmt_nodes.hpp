//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_STMT_NODES_HPP
#define NAIVE_PASCAL_COMPILER_STMT_NODES_HPP

#include "dummy_node.hpp"
#include "type_nodes.hpp"
#include "routine_nodes.hpp"
#include "const_value_nodes.hpp"

namespace npc
{
    class StmtNode;
    class AbstractStmtNode : public DummyNode
    {
    public:
        ~AbstractStmtNode() override = 0;
    };

    inline AbstractStmtNode::~AbstractStmtNode() = default;

    class StmtNode : public DummyNode
    {
    public:
        StmtNode(uint32_t label, const NodePtr &stmt)
                : _labeled(true),
                  _label(label),
                  stmt(cast_node<AbstractStmtNode>(stmt))
        {
        }

        StmtNode(const NodePtr &stmt)
                : _labeled(false),
                  _label(0),
                  stmt(cast_node<AbstractStmtNode>(stmt))
        {
        }

        std::shared_ptr<AbstractStmtNode> stmt;

        bool isLabeled()
        {
            return this->_labeled;
        }

        uint32_t label()
        {
            return _label;
        }

    private:
        bool _labeled;
        uint32_t _label;
    };

    enum class DirectionEnum
    {
        TO,
        DOWNTO
    };

    class DirectionNode : public DummyNode
    {
    public:
        DirectionEnum val;

        DirectionNode(bool isDownTo)
                : val(isDownTo ? DirectionEnum::DOWNTO : DirectionEnum::TO)
        {}
    };

    class AssignStmtNode : public AbstractStmtNode
    {
    public:
        AssignStmtNode(NodePtr lhs, NodePtr rhs, bool compound=false)
                : lhs(lhs), rhs(rhs), compound(compound) {}
        NodePtr lhs, rhs;
        bool compound;
    };

    class ProcStmtNode : public AbstractStmtNode
    {
    public:
        ProcStmtNode(std::shared_ptr<ProcCallNode> proc_call) : proc_call(proc_call), syscall(false) {}
        ProcStmtNode(std::shared_ptr<SysProcCallNode> sys_proc_call) : sys_proc_call(sys_proc_call), syscall(true) {}
        std::shared_ptr<ProcCallNode> proc_call;
        std::shared_ptr<SysProcCallNode> sys_proc_call;
        bool syscall;
    };

    class CompoundStmtNode : public AbstractStmtNode
    {
    };

    class IfStmtNode : public AbstractStmtNode
    {
    public:
        IfStmtNode(NodePtr &_expr, NodePtr &_stmt, NodePtr &_else_stmt)
                : expr(cast_node<ExprNode>(_expr)), stmt(cast_node<StmtNode>(_stmt))
                , else_stmt(cast_node<StmtNode>(_else_stmt)) {}
        std::shared_ptr<ExprNode> expr;
        std::shared_ptr<StmtNode> stmt;
        std::shared_ptr<StmtNode> else_stmt;
    };

    class RepeatStmtNode : public AbstractStmtNode
    {
    public:
        RepeatStmtNode(NodePtr &_stmts, NodePtr &_expr)
                : expr(cast_node<ExprNode>(_expr)) {
            this->merge_children(_stmts->children());
        }
        std::shared_ptr<ExprNode> expr;
    };

    class WhileStmtNode : public AbstractStmtNode
    {
    public:
        WhileStmtNode(NodePtr &_expr, NodePtr &_stmt)
        : expr(cast_node<ExprNode>(_expr)), stmt(cast_node<StmtNode>(_stmt)) {}
        std::shared_ptr<ExprNode> expr;
        std::shared_ptr<StmtNode> stmt;
    };

    class ForStmtNode : public AbstractStmtNode
    {
    public:
        ForStmtNode(NodePtr _assign, NodePtr &_dir, NodePtr &_expr, NodePtr &_stmt)
        : assign(cast_node<AssignStmtNode>(_assign)), dir(cast_node<DirectionNode>(_dir))
        , expr(cast_node<ExprNode>(_expr)), stmt(cast_node<StmtNode>(_stmt)) {}
        std::shared_ptr<AssignStmtNode> assign;
        std::shared_ptr<DirectionNode> dir;
        std::shared_ptr<ExprNode> expr;
        std::shared_ptr<StmtNode> stmt;
    };

    class CaseStmtNode : public AbstractStmtNode
    {
    public:
        CaseStmtNode(NodePtr &_stmts, NodePtr &_expr)
        : expr(cast_node<ExprNode>(_expr)) {
            this->merge_children(_stmts->children());
        }
        std::shared_ptr<ExprNode> expr;
    };

    class GotoStmtNode : public AbstractStmtNode
    {
    public:
        GotoStmtNode(NodePtr &_integer) : integer(cast_node<IntegerNode>(_integer)) {}
        std::shared_ptr<IntegerNode> integer;
    };

    class DummyStmtNode : public AbstractStmtNode
    {
    };

    class StmtList : public DummyNode
    {};
}

#endif //NAIVE_PASCAL_COMPILER_STMT_NODES_HPP
