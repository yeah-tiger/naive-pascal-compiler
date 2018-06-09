//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_PROCEDURE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_PROCEDURE_NODES_HPP

#include "dummy_node.hpp"

namespace npc
{
    class ParametersNode;

    class TypeDeclNode;

    class RoutineNode : public DummyNode
    {
    };

    class SubRoutineNode : public DummyNode
    {
    };

    class ProcedureNode : public DummyNode
    {
    };

    class FunctionHeadNode : public DummyNode
    {
        std::shared_ptr<ParametersNode> parameters;
        std::shared_ptr<TypeDeclNode> type;
    };

    class FunctionNode : public DummyNode
    {
    public:
        std::shared_ptr<FunctionHeadNode> head;
    };
}

#endif //NAIVE_PASCAL_COMPILER_PROCEDURE_NODES_HPP
