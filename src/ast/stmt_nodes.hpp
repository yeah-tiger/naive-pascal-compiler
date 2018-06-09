//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_STMT_NODES_HPP
#define NAIVE_PASCAL_COMPILER_STMT_NODES_HPP

#include "dummy_node.hpp"

namespace npc
{
    class AbstractStmtNode : public DummyNode
    {
    public:
        ~AbstractStmtNode() override = 0;
    };

    inline AbstractStmtNode::~AbstractStmtNode() = default;

    class AssignStmtNode : public AbstractStmtNode
    {
    };

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
}

#endif //NAIVE_PASCAL_COMPILER_STMT_NODES_HPP
