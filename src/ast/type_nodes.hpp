//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP

#include <cassert>
#include <vector>
#include "dummy_node.hpp"

namespace npc
{
    class NameList;
    class VarDeclList;
    class VarDeclNode;
    class IdentifierNode;
    class TypeDeclNode;
    class TypePartNode;
    class TypeDefNode;

    class NameList : public DummyNode {};
    class VarDeclList : public DummyNode {};
    class IdentifierNode : public DummyNode
    {
    public:
        IdentifierNode(std::string s) : name(std::move(s)) {}
        IdentifierNode(const char *c) : name(c) {}
        std::string name;
    };
    class TypeDeclNode : public DummyNode
    {
    public:
        enum Type { error, integer, real, character, boolean, array, record };
        TypeDeclNode(Type type) : type(type) {}
        Type type;
    };
    class VarDeclNode : public DummyNode
    {
    public:
        std::shared_ptr<AbstractNode> identifier, type;
        VarDeclNode(std::shared_ptr<AbstractNode> i, std::shared_ptr<AbstractNode> t) : identifier(std::move(i)), type(std::move(t)) {}
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
