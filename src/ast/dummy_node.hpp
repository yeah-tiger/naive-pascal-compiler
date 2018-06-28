//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_DUMMY_NODE_H
#define NAIVE_PASCAL_COMPILER_DUMMY_NODE_H

#include "ast/abstract_node.hpp"

namespace npc
{
    /*
     * minimal AST node with nothing to do.
     */
    class DummyNode : public AbstractNode
    {
    public:
        llvm::Value *codegen(CodegenContext &context) override
        {
            std::cout << typeid(*this).name() << std::endl;
            assert(false);
            return nullptr;
        }

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "<unspecified-from-dummy>")"};
        }
    };
}

#endif //NAIVE_PASCAL_COMPILER_DUMMY_NODE_H
