//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_ROUTINE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_ROUTINE_NODES_HPP

#include "dummy_node.hpp"
#include "parameter_nodes.hpp"
#include "type_nodes.hpp"
#include "expr_nodes.hpp"
#include "stmt_nodes.hpp"
#include "head_list_nodes.hpp"

namespace npc
{
    class RoutineNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<HeadListNode> head_list;

        explicit RoutineNode(const NodePtr &_name)
                : name(cast_node<IdentifierNode>(_name))
        {}

        void add_stmt_list(const NodePtr &stmt_list)
        {
            assert(is_a_ptr_of<StmtList>(stmt_list));
            this->children() = stmt_list->children();
        }
    };

    class ProgramNode : public RoutineNode
    {
    public:
        explicit ProgramNode(const NodePtr &_name)
                : RoutineNode(_name)
        {}
    };

    class ProcedureNode : public RoutineNode
    {
    public:
        std::shared_ptr<ParametersNode> params;

        ProcedureNode(const NodePtr &_name, const NodePtr &_params)
                : RoutineNode(_name), params(cast_node<ParametersNode>(_params))
        {}
    };

    class FunctionNode : public RoutineNode
    {
    public:
        std::shared_ptr<ParametersNode> params;
        std::shared_ptr<SimpleTypeDeclNode> type;

        FunctionNode(const NodePtr &_name, const NodePtr &_params, const NodePtr &_type)
                : RoutineNode(_name), params(cast_node<ParametersNode>(_params)),
                  type(cast_node<SimpleTypeDeclNode>(_type))
        {}
    };
}

#endif //NAIVE_PASCAL_COMPILER_ROUTINE_NODES_HPP
