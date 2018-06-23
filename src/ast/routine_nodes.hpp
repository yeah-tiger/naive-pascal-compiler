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
        std::string json_head() const override
        {
            return std::string{R"("type": "SubroutineList")"};
        }

        bool should_have_children() const final
        { return true; }
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
        std::string json_head() const override
        {
            return std::string{R"("type": "HeadList", "consts": )"} +
                   this->const_list->to_json() +
                   R"(, "types": )" +
                   this->type_list->to_json() +
                   R"(, "vars": )" +
                   this->var_list->to_json() +
                   R"(, "subroutines": )" +
                   this->subroutine_list->to_json();
        }

        bool should_have_children() const final
        { return false; }
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
        std::string json_head() const override
        {
            return std::string{R"("type": "Rsoutine", "name": )"} +
                   this->name->to_json() +
                   R"(, "head": )" +
                   this->head_list->to_json();
        }

        bool should_have_children() const final
        { return true; }
    };

    class ProgramNode : public RoutineNode
    {
    public:
        using RoutineNode::RoutineNode;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "Program", "name": )"} +
                   this->name->to_json() +
                   R"(, "head": )" +
                   this->head_list->to_json();
        }
    };

    class ProcedureNode : public RoutineNode
    {
    public:
        std::shared_ptr<ParamListNode> params;

        ProcedureNode(const NodePtr &name, const NodePtr &params, const NodePtr &head_list)
                : RoutineNode(name, head_list), params(cast_node<ParamListNode>(params))
        {}

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "Procedure", "name": )"} +
                   this->name->to_json() +
                   R"(, "params": )" +
                   this->params->to_json() +
                   R"(, "head": )" +
                   this->head_list->to_json();
        }
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

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "Function", "name": )"} +
                   this->name->to_json() +
                   R"(, "params": )" +
                   this->params->to_json() +
                   R"(, "return": )" +
                   this->type->to_json() +
                   R"(, "head": )" +
                   this->head_list->to_json();
        }
    };
}

#endif //NAIVE_PASCAL_COMPILER_ROUTINE_NODES_HPP
