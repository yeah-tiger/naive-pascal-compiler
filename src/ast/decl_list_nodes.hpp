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

    class ParamDeclNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<SimpleTypeNode> type;

        ParamDeclNode(const NodePtr &name, const NodePtr &type)
                : name(cast_node<IdentifierNode>(name)), type(cast_node<SimpleTypeNode>(type))
        {}

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "ParamDecl", "name": )"} +
                    this->name->to_json() +
                   R"(, "decl": )" +
                    this->type->to_json();
        }
    };

    class ParamListNode : public DummyNode
    {
    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "ParamList")"};
        }
    };

    class VarDeclNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<TypeNode> type;

        VarDeclNode(const NodePtr &name, const NodePtr &type)
                : name(cast_node<IdentifierNode>(name)), type(cast_node<TypeNode>(type))
        {}

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "VarDecl", "name": )"} +
                    this->name->to_json() +
                   R"(, "decl": )" +
                    this->type->to_json();
        }
    };

    class VarListNode : public DummyNode
    {
    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "VarList")"};
        }
    };

    class ConstDeclNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<ConstValueNode> value;

        ConstDeclNode(const NodePtr &name, const NodePtr &value)
                : name(cast_node<IdentifierNode>(name)), value(cast_node<ConstValueNode>(value))
        {}

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "ConstDecl", "name": )"} +
                    this->name->to_json() +
                   R"(, "value": )" +
                    this->value->to_json();
        }
    };

    class ConstListNode : public DummyNode
    {
    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "ConstList")"};
        }
    };

    class TypeDefNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<TypeNode> type;

        TypeDefNode(const NodePtr &name, const NodePtr &type)
                : name(cast_node<IdentifierNode>(name)), type(cast_node<TypeNode>(type))
        {}

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "TypeDef", "name": )"} +
                    this->name->to_json() +
                   R"(, "alias": )" +
                    this->type->to_json();
        }
    };

    class TypeListNode : public DummyNode
    {
    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "TypeList")"};
        }
    };
}

#endif //NAIVE_PASCAL_COMPILER_DECL_LIST_NODES_HPP
