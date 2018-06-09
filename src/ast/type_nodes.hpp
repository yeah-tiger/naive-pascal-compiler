//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP

#include <cassert>
#include "dummy_node.hpp"

namespace npc
{
    class TypeDeclNode : public DummyNode
    {
    };

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
            assert(is_a_ptr_of<NameNode>(name));
            assert(is_a_ptr_of<TypeDeclNode>(type_decl));
        }
    };
}

#endif //NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP
