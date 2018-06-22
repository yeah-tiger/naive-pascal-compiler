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

// TODO: Refactor all these type nodes

namespace npc
{
    enum class Type
    {
        error, integer, real, character, boolean, array, record
    };

    class TypeNode : public DummyNode
    {
    public:
        ~TypeNode() override = 0;

        virtual Type getTypeClass() const
        {
            return Type::error;  // TODO: should be pure virtual
        }
    };

    inline TypeNode::~TypeNode() = default;

    /*
     * for system built-in types
     */
    class SimpleTypeNode : public TypeNode
    {
    public:
        SimpleTypeNode(Type type) : type(type)
        {}

        Type type;

        Type getTypeClass() const override
        {
            return this->type;
        }
    };

    class RecordTypeNode : public TypeNode  // TODO: check all children are FieldDeclNode
    {
    public:
        Type getTypeClass() const override
        {
            // TODO
            assert(false);
        }
    };

    class AliasTypeNode : public TypeNode
    {
    public:
        AliasTypeNode(const NodePtr &name_node)
                : _name(std::dynamic_pointer_cast<IdentifierNode>(name_node)->name)
        {}

        const std::string &name() const noexcept
        {
            return this->_name;
        }

    private:
        std::string _name;
    };

    class EnumTypeNode : public TypeNode
    {
    };

    class RangeTypeNode : public TypeNode
    {
    public:
        const NodePtr &min, &max;

        // TODO: checking type of both const_value is same.
        RangeTypeNode(const NodePtr &min, const NodePtr &max)
                : min(min), max(max)
        {}
    };

    class ArrayTypeNode : public TypeNode
    {
    };
}

#endif //NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP
