//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP

#include <cassert>
#include <utility>
#include <vector>
#include "dummy_node.hpp"
#include "identifier_node.hpp"
#include "expr_nodes.hpp"

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
                {Type::BOOLEAN, "boolean"},
                {Type::INTEGER, "integer"},
                {Type::REAL, "real"},
                {Type::CHAR, "char"},
                {Type::STRING, "string"},
                {Type::ARRAY, "array"},
                {Type::RECORD, "record"},
                {Type::SET, "set"}
        };
        // TODO: bound checking
        return type_to_string[type];
    }

    inline Type to_type(std::string type)
    {
        std::map<std::string, Type> string_to_type{
                {"<undefined-type>", Type::UNDEFINED},
                {"boolean", Type::BOOLEAN},
                {"integer", Type::INTEGER},
                {"real", Type::REAL},
                {"char", Type::CHAR},
                {"string", Type::STRING},
                {"array", Type::ARRAY},
                {"record", Type::RECORD},
                {"set", Type::SET}
        };
        // TODO: bound checking
        return string_to_type[type];
    }

    class TypeNode : public DummyNode
    {
    public:
        Type type = Type::UNDEFINED;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "Type", "name": ")"} +
                    to_string(this->type) + "\"";
        }
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
