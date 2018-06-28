//
// Created by 孙耀珠 on 2018/6/23.
//

#ifndef NAIVE_PASCAL_COMPILER_DECL_LIST_NODES_HPP
#define NAIVE_PASCAL_COMPILER_DECL_LIST_NODES_HPP

#include "ast/dummy_node.hpp"
#include "ast/identifier_node.hpp"
#include "ast/type_nodes.hpp"
#include "ast/const_value_nodes.hpp"

namespace npc
{
    class ArgListNode : public DummyNode
    {
    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "ArgList")"};
        }

        bool should_have_children() const override
        { return true; }
    };

    class ParamDeclNode : public DummyNode
    {
    public:
        std::shared_ptr<IdentifierNode> name;
        std::shared_ptr<TypeNode> type;

        ParamDeclNode(const NodePtr &name, const NodePtr &type)
                : name(cast_node<IdentifierNode>(name)), type(cast_node<TypeNode>(type))
        {
            assert(is_a_ptr_of<SimpleTypeNode>(type) || is_a_ptr_of<AliasTypeNode>(type));
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

        llvm::Value *codegen(CodegenContext &context) override;

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
    public:
        llvm::Value *codegen(CodegenContext &context) override;

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

        llvm::Value *codegen(CodegenContext &context) override;

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
    public:
        llvm::Value *codegen(CodegenContext &context) override;

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

        llvm::Value *codegen(CodegenContext &context) override;

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
    public:
        llvm::Value *codegen(CodegenContext &context) override;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "TypeList")"};
        }

        bool should_have_children() const override
        { return true; }
    };

    // Intermediate container, would be removed
    class NameListNode : public DummyNode
    {
    };
}

#endif //NAIVE_PASCAL_COMPILER_DECL_LIST_NODES_HPP
