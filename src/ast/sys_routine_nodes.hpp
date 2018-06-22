//
// Created by 孙耀珠 on 2018/6/22.
//

#ifndef NAIVE_PASCAL_COMPILER_SYS_ROUTINE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_SYS_ROUTINE_NODES_HPP

#include "dummy_node.hpp"
#include "expr_nodes.hpp"

namespace npc
{
    enum class SysRoutine
    {
        ABS, CHR, ORD, PRED, SQR, SQRT, SUCC,
        READ_FUN, READLN,  // READ is occupied by flex part.
        WRITE, WRITELN
    };

    class SysRoutineNode : public DummyNode
    {
    public:
        SysRoutine routine;

        explicit SysRoutineNode(SysRoutine routine) : routine(routine) {}
    };

    class SysCallNode : public DummyNode
    {
    public:
        std::shared_ptr<SysRoutineNode> routine;
        std::shared_ptr<ExprListNode> args;

        SysCallNode(const NodePtr &routine, const NodePtr &args)
                : routine(cast_node<SysRoutineNode>(routine)), args(cast_node<ExprListNode>(args))
        {}
    };
}

#endif //NAIVE_PASCAL_COMPILER_SYS_ROUTINE_NODES_HPP
