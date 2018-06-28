//
// Created by 孙耀珠 on 2018/6/22.
//

#ifndef NAIVE_PASCAL_COMPILER_ROUTINE_CALL_NODES_HPP
#define NAIVE_PASCAL_COMPILER_ROUTINE_CALL_NODES_HPP

#include "ast/dummy_node.hpp"
#include "ast/identifier_node.hpp"
#include "ast/decl_list_nodes.hpp"

namespace npc
{
    class RoutineCallNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> identifier;
        std::shared_ptr<ArgListNode> args;

        RoutineCallNode(const NodePtr &identifier, const NodePtr &args)
                : identifier(cast_node<IdentifierNode>(identifier)), args(cast_node<ArgListNode>(args))
        {}

        explicit RoutineCallNode(const NodePtr &identifier)
                : RoutineCallNode(identifier, make_node<ArgListNode>())
        {}

        llvm::Value *codegen(CodegenContext &context) override;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "RoutineCall", "identifier": )"} + this->identifier->to_json() +
                   R"(, "args": )" + this->args->to_json();
        }

        bool should_have_children() const final
        { return false; }
    };
}

#endif //NAIVE_PASCAL_COMPILER_ROUTINE_CALL_NODES_HPP
