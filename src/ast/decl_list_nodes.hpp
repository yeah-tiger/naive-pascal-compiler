//
// Created by 孙耀珠 on 2018/6/23.
//

#ifndef NAIVE_PASCAL_COMPILER_DECL_LIST_NODES_HPP
#define NAIVE_PASCAL_COMPILER_DECL_LIST_NODES_HPP

#include "dummy_node.hpp"
#include "identifier_node.hpp"
#include "type_nodes.hpp"
#include "const_value_nodes.hpp"

namespace npc
{
    class NameListNode : public DummyNode
    {
    };

    class ParamDeclNode : public DummyNode {
    public:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<SimpleTypeNode> type;

        ParamDeclNode(const NodePtr &name, const NodePtr &type)
                : name(cast_node<IdentifierNode>(name)), type(cast_node<SimpleTypeNode>(type))
        {}
    };

    class ParamListNode : public DummyNode
    {
    };

    class VarDeclNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<TypeNode> type;

        VarDeclNode(const NodePtr &name, const NodePtr &type)
                : name(cast_node<IdentifierNode>(name)), type(cast_node<TypeNode>(type))
        {}
    };

    class VarListNode : public DummyNode
    {
    };

    class ConstExprNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<ConstValueNode> value;

        ConstExprNode(const NodePtr &name, const NodePtr &value)
                : name(cast_node<IdentifierNode>(name)), value(cast_node<ConstValueNode>(value))
        {}
    };

    class ConstListNode : public DummyNode
    {
    };

    class TypeDefNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<TypeNode> type;

        TypeDefNode(const NodePtr &name, const NodePtr &type)
                : name(cast_node<IdentifierNode>(name)), type(cast_node<TypeNode>(type))
        {}
    };

    class TypeListNode : public DummyNode
    {
    };
}

#endif //NAIVE_PASCAL_COMPILER_DECL_LIST_NODES_HPP
