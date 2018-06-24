//
// Created by 孙耀珠 on 2018/6/22.
//

#ifndef NAIVE_PASCAL_COMPILER_IDENTIFIER_NODE_HPP
#define NAIVE_PASCAL_COMPILER_IDENTIFIER_NODE_HPP

#include "dummy_node.hpp"
#include "expr_nodes.hpp"
#include "type_nodes.hpp"

namespace npc
{
    class IdentifierNode : public ExprNode
    {
    public:
        std::string name;

        explicit IdentifierNode(std::string s) : name(std::move(s))
        {}

        explicit IdentifierNode(const char *c) : name(c)
        {}

        virtual Type get_type() const {
            auto v_name = name;
            auto type_var = AbstractNode::sym_table.find(v_name, true);
            if (type_var == nullptr) {
                std::cerr << v_name << " has not been declared.\n" << std::endl;
                assert(false);
            }
            return to_type(type_var->type);
        }

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "Identifier", "name": ")"}
                   + this->name + "\"";
        }

        bool should_have_children() const override
        { return false; }
    };
}

#endif //NAIVE_PASCAL_COMPILER_IDENTIFIER_NODE_HPP
