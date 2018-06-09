//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_DUMMY_NODE_H
#define NAIVE_PASCAL_COMPILER_DUMMY_NODE_H

#include <string>
#include "abstract_node.hpp"

namespace npc
{
    /*
     * minimal AST node with nothing to do.
     */
    class DummyNode : public AbstractNode
    {
    public:
        std::string toString() const override
        {
            return std::string{};
        }

        void codegen(CodegenContext &context) const override
        {
        }
    };
}

#endif //NAIVE_PASCAL_COMPILER_DUMMY_NODE_H
