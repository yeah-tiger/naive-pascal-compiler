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
    protected:
        bool should_have_children() const override
        { return true; }

        std::string json_head() const override
        {
            return std::string{R"("type": "NameList")"};
        }
    };

    class ParamDeclNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<SimpleTypeNode> type;

        ParamDeclNode(const NodePtr &name, const NodePtr &type)
                : name(cast_node<IdentifierNode>(name)), type(cast_node<SimpleTypeNode>(type))
        {}

        void type_check() {
            auto v_name = name->name, v_type = to_string(type->type);
            if (sym_table.find(v_name) != nullptr) {
                std::cerr << v_name << " already declared." << std::endl;
                assert(false);
            }
            sym_table.insert(v_name, v_type);
        }

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "ParamDecl", "name": )"} +
                   this->name->to_json() +
                   R"(, "decl": )" +
                   this->type->to_json();
        }

        bool should_have_children() const override
        { return false; }
    };

    class ParamListNode : public DummyNode
    {
    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "ParamList")"};
        }

        bool should_have_children() const override
        { return true; }
    };

    class VarDeclNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<TypeNode> type;

        VarDeclNode(const NodePtr &name, const NodePtr &type)
                : name(cast_node<IdentifierNode>(name)), type(cast_node<TypeNode>(type))
        {}

        void type_check() {
            if (type->type == Type::UNDEFINED) {
                std::cerr << "Using undefined type!" << std::endl;
                assert(false);
            }
            auto v_name = name->name, v_type = to_string(type->type);
            if (sym_table.find(v_name) != nullptr) {
                std::cerr << v_name << " already declared." << std::endl;
                assert(false);
            }
            sym_table.insert(v_name, v_type);
        }

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "VarDecl", "name": )"} +
                   this->name->to_json() +
                   R"(, "decl": )" +
                   this->type->to_json();
        }

        bool should_have_children() const override
        { return false; }
    };

    class VarListNode : public DummyNode
    {
    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "VarList")"};
        }

        bool should_have_children() const override
        { return true; }
    };

    class ConstDeclNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<ConstValueNode> value;

        ConstDeclNode(const NodePtr &name, const NodePtr &value)
                : name(cast_node<IdentifierNode>(name)), value(cast_node<ConstValueNode>(value))
        {}

        void type_check() {
            auto v_name = name->name, v_type = to_string(value->type);
            if (sym_table.find(v_name) != nullptr) {
                std::cerr << v_name << " already declared." << std::endl;
                assert(false);
            }
            sym_table.insert(v_name, v_type);
        }

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "ConstDecl", "name": )"} +
                   this->name->to_json() +
                   R"(, "value": )" +
                   this->value->to_json();
        }

        bool should_have_children() const override
        { return false; }
    };

    class ConstListNode : public DummyNode
    {
    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "ConstList")"};
        }

        bool should_have_children() const override
        { return true; }
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

        bool should_have_children() const override
        { return false; }
    };

    class TypeListNode : public DummyNode
    {
    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "TypeList")"};
        }

        bool should_have_children() const override
        { return true; }
    };
}

#endif //NAIVE_PASCAL_COMPILER_DECL_LIST_NODES_HPP
