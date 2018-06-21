//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_CONST_EXPR_NODE_HPP
#define NAIVE_PASCAL_COMPILER_CONST_EXPR_NODE_HPP

#include "dummy_node.hpp"
#include "const_value_nodes.hpp"

namespace npc
{
    class ConstExprNode : public DummyNode
    {
    public:
        std::shared_ptr<AbstractNode> name;
        std::shared_ptr<ConstValueNode> value;

        ConstExprNode(const NodePtr &nameNode, const NodePtr &constValueNode)
                : name(nameNode), value(cast_node<ConstValueNode>(constValueNode))
        {}

        void codegen(CodegenContext &context) const override
        {
            if (this->_children.size() != 0)
            {
                // TODO:
            }
        }
    };
}


#endif //NAIVE_PASCAL_COMPILER_CONST_EXPR_NODE_HPP
