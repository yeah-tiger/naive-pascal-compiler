//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_DUMMY_NODE_H
#define NAIVE_PASCAL_COMPILER_DUMMY_NODE_H

#include <string>
#include <sstream>
#include "abstract_node.hpp"

namespace npc
{
    /*
     * minimal AST node with nothing to do.
     */
    class DummyNode : public AbstractNode
    {
    public:
        void codegen(CodegenContext &context) const override
        {
        }

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "<unspecified-from-dummy>")"};
        }
    };

    // TODO: Move back to expr_nodes.hpp
    enum class Type;
    inline Type to_type(std::string type);
    class ExprNode : public DummyNode
    {
    public:
        virtual Type get_type() const {
            std::cerr << "Un-implemented get_type()" << std::endl;
            assert(false);
        }
    };

    class ExprListNode : public DummyNode
    {
    };
}

#endif //NAIVE_PASCAL_COMPILER_DUMMY_NODE_H
