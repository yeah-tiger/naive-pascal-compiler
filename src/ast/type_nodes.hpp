//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP

#include "dummy_node.hpp"

namespace npc
{
    class TypePartNode : public DummyNode
    {
    };

    class TypeDefNode : public DummyNode
    {
    public:
        TypeDefNode(const NodePtr &name, const NodePtr &type_decl)
        {
            this->add(name);
            this->add(type_decl);
        }
    };
}

#endif //NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP
