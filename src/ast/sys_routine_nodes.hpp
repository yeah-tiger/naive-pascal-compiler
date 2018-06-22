//
// Created by 孙耀珠 on 2018/6/22.
//

#ifndef NAIVE_PASCAL_COMPILER_SYS_ROUTINE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_SYS_ROUTINE_NODES_HPP

#include "dummy_node.hpp"

namespace npc
{
    enum class SysFunction
    {
        ABS, CHR, ORD, PRED, SQR, SQRT, SUCC,
        READ_FUN, READLN,  // READ is occupied by flex part.
        WRITE, WRITELN
    };

    class SysFunctionNode : public DummyNode
    {
    public:
        SysFunction function;

        SysFunctionNode(SysFunction _function)
                : function(_function)
        {}
    };
}

#endif //NAIVE_PASCAL_COMPILER_SYS_ROUTINE_NODES_HPP
