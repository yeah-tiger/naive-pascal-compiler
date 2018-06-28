//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP

#include "ast/dummy_node.hpp"
#include "ast/abstract_subnodes.hpp"
#include "ast/identifier_node.hpp"

namespace npc
{
    enum class Type
    {
        UNDEFINED,
        BOOLEAN, INTEGER, REAL, CHAR,
        STRING, ARRAY, RECORD, SET
    };

    inline std::string to_string(Type type)
    {
        std::map<Type, std::string> type_to_string{
                {Type::UNDEFINED, "<undefined-type>"},
                {Type::BOOLEAN,   "boolean"},
                {Type::INTEGER,   "integer"},
                {Type::REAL,      "real"},
                {Type::CHAR,      "char"},
                {Type::STRING,    "string"},
                {Type::ARRAY,     "array"},
                {Type::RECORD,    "record"},
                {Type::SET,       "set"}
        };
        // TODO: bound checking
        return type_to_string[type];
    }

    class TypeNode : public DummyNode
    {
    public:
        Type type = Type::UNDEFINED;

        llvm::Type *get_llvm_type(CodegenContext &context) const;
        llvm::Constant *get_default_value(CodegenContext &context) const;

    protected:
        TypeNode() = default;

        std::string json_head() const override
        {
            return std::string{R"("type": "Type", "name": ")"} +
                   to_string(this->type) + "\"";
        }

        bool should_have_children() const override
        { return false; }
    };

    class SimpleTypeNode : public TypeNode
    {
    public:
        SimpleTypeNode(Type type)
        {
            this->type = type;
        }
    };

    class AliasTypeNode : public TypeNode
    {
    public:
        std::shared_ptr<IdentifierNode> identifier;

        AliasTypeNode(const NodePtr &identifier)
                : identifier(cast_node<IdentifierNode>(identifier))
        {
            // TODO: assert the identifier is a type
        }

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "Type", "name": "alias", "identifier": )"} +
                   to_string(this->type);
        }
    };

    class RangeNode : public DummyNode
    {
    public:
        std::shared_ptr<ExprNode> min;
        std::shared_ptr<ExprNode> max;

        RangeNode(const NodePtr &min, const NodePtr &max)
                : min(cast_node<ExprNode>(min)), max(cast_node<ExprNode>(max))
        {
            // TODO: assert both types are the same
        }
    };

    class ArrayTypeNode : public TypeNode
    {
    public:
        std::shared_ptr<RangeNode> range;
        std::shared_ptr<TypeNode> element_type;

        ArrayTypeNode(const NodePtr &range, const NodePtr &element_type)
                : range(cast_node<RangeNode>(range)), element_type(cast_node<TypeNode>(element_type))
        {
            type = Type::ARRAY;
        }
    };

    class RecordTypeNode : public TypeNode
    {
    public:
        RecordTypeNode()
        {
            type = Type::RECORD;
        }
    };

    class SetTypeNode : public TypeNode
    {
    public:
        SetTypeNode()
        {
            type = Type::SET;
        }
    };
}

#endif //NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP
