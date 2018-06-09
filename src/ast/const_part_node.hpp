//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_CONST_PART_NODE_HPP
#define NAIVE_PASCAL_COMPILER_CONST_PART_NODE_HPP

#include "../utils/ast_utils.hpp"
#include "dummy_node.hpp"
#include "const_expr_node.hpp"

namespace npc
{
    class ConstPartNode : public DummyNode
    {
    public:
        void codegen(CodegenContext &context) const override
        {
            for (auto &node : this->_children)
            {
                if (!is_a_ptr_of<ConstExprNode>(node))
                {
                    // TODO
                }
            }
        }
    };
}

#endif //NAIVE_PASCAL_COMPILER_CONST_PART_NODE_HPP
