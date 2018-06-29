//
// Created by 孙耀珠 on 2018/6/22.
//

#ifndef NAIVE_PASCAL_COMPILER_IDENTIFIER_NODE_HPP
#define NAIVE_PASCAL_COMPILER_IDENTIFIER_NODE_HPP

#include <algorithm>

#include "ast/abstract_subnodes.hpp"

namespace npc
{
    class IdentifierNode : public ExprNode
    {
    public:
        std::string name;

        explicit IdentifierNode(const char *c) : name(c)
        {
            std::transform(name.begin(), name.end(), name.begin(), tolower);
        }

        llvm::Value *get_ptr(CodegenContext &context);
        llvm::Value *codegen(CodegenContext &context) override;

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
