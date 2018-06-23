//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_ROUTINE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_ROUTINE_NODES_HPP

#include "dummy_node.hpp"
#include "identifier_node.hpp"
#include "type_nodes.hpp"
#include "decl_list_nodes.hpp"

namespace npc
{
    class SubroutineListNode : public DummyNode
    {
    protected:
        std::string jsonHead() const override
        {
            return std::string{R"EOF("type": "subroutineList",)EOF"};
        }
    };

    class HeadListNode : public DummyNode
    {
    public:
        std::shared_ptr<ConstListNode> const_list;
        std::shared_ptr<TypeListNode> type_list;
        std::shared_ptr<VarListNode> var_list;
        std::shared_ptr<SubroutineListNode> subroutine_list;

        HeadListNode(const NodePtr &consts, const NodePtr &types, const NodePtr &vars, const NodePtr &subroutines)
                : const_list(cast_node<ConstListNode>(consts)), type_list(cast_node<TypeListNode>(types)),
                  var_list(cast_node<VarListNode>(vars)), subroutine_list(cast_node<SubroutineListNode>(subroutines))
        {}

    protected:
        bool should_have_children() const override
        { return false; }

        std::string jsonHead() const override
        {
            return std::string{R"EOF("type": "headList", "constList": )EOF"} +
                   this->const_list->toJson() +
                   R"(, "typeList": )" +
                   this->type_list->toJson() +
                   R"(, "varList": )" +
                   this->var_list->toJson() +
                   R"(, "subroutineList": )" +
                   this->subroutine_list->toJson() + ",";
        }
    };

    class RoutineNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<HeadListNode> head_list;

        RoutineNode(const NodePtr &name, const NodePtr &head_list)
                : name(cast_node<IdentifierNode>(name)), head_list(cast_node<HeadListNode>(head_list))
        {}

    protected:
        std::string jsonHead() const override
        {
            return std::string{R"EOF("type": "routine", "name": )EOF"} +
                   this->name->toJson() +
                   R"(, "headList": )" +
                   this->head_list->toJson() + ",";
        }
    };

    class ProgramNode : public RoutineNode
    {
    public:
        using RoutineNode::RoutineNode;
    };

    class ProcedureNode : public RoutineNode
    {
    public:
        std::shared_ptr<ParamListNode> params;

        ProcedureNode(const NodePtr &name, const NodePtr &params, const NodePtr &head_list)
                : RoutineNode(name, head_list), params(cast_node<ParamListNode>(params))
        {}
    };

    class FunctionNode : public RoutineNode
    {
    public:
        std::shared_ptr<ParamListNode> params;
        std::shared_ptr<SimpleTypeNode> type;

        FunctionNode(const NodePtr &name, const NodePtr &params, const NodePtr &type, const NodePtr &head_list)
                : RoutineNode(name, head_list), params(cast_node<ParamListNode>(params)),
                  type(cast_node<SimpleTypeNode>(type))
        {}
    };
}

#endif //NAIVE_PASCAL_COMPILER_ROUTINE_NODES_HPP
