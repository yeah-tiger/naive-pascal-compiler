//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_CONST_EXPR_NODE_HPP
#define NAIVE_PASCAL_COMPILER_CONST_EXPR_NODE_HPP

#include "dummy_node.hpp"

namespace npc
{
    class ConstExprNode : public DummyNode
    {
    public:
        ConstExprNode(const NodePtr &nameNode, const NodePtr &constValueNode)
        {
            this->add(nameNode);
            this->add(constValueNode);
        }

        void codegen(CodegenContext &context) const override
        {
            if (this->_children.size() != 2)
            {
                // TODO:
            }
        }
    };
}


#endif //NAIVE_PASCAL_COMPILER_CONST_EXPR_NODE_HPP
