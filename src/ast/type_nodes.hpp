//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP

#include <cassert>
#include <utility>
#include <vector>
#include "dummy_node.hpp"

namespace npc
{
    // TODO: rearrange file structure.

    class NameListNode;

    class VarDeclListNode;

    class VarDeclNode;

    class IdentifierNode;

    class SimpleTypeDeclNode;

    class TypePartNode;

    class TypeDefNode;

    class ArrayRefNode;

    class AbstractExprNode;

    class ExprListNode;

    enum class Type
    {
        error, integer, real, character, boolean, array, record
    };

    class NameListNode : public DummyNode
    {
    };

    class VarDeclListNode : public DummyNode
    {
    };

    class IdentifierNode : public DummyNode
    {
    public:
        IdentifierNode(std::string s) : name(std::move(s))
        {}

        IdentifierNode(const char *c) : name(c)
        {}

        std::string name;
    };

    class AbstractTypeNode : public DummyNode
    {
    public:
        ~AbstractTypeNode() override = 0;

        virtual Type getTypeClass() const
        {
            return Type::error;  // TODO: should be pure virtual
        }
    };

    inline AbstractTypeNode::~AbstractTypeNode() = default;

    /*
     * for system built-in types
     */
    class SimpleTypeDeclNode : public AbstractTypeNode
    {
    public:
        SimpleTypeDeclNode(Type type) : type(type)
        {}

        Type type;

        Type getTypeClass() const override
        {
            return this->type;
        }
    };

    class VarDeclNode : public DummyNode
    {
    public:
        std::shared_ptr<AbstractNode> identifier, type;

        VarDeclNode(std::shared_ptr<AbstractNode> i, std::shared_ptr<AbstractNode> t) : identifier(std::move(i)),
                                                                                        type(std::move(t))
        {}
    };

    class TypePartNode : public DummyNode
    {
    };

    class FieldDeclNode : public DummyNode
    {
    public:
        NodePtr nameList;
        NodePtr typeDecl;

        FieldDeclNode(const NodePtr &name_list, const NodePtr &type_decl)
                : nameList(name_list),
                  typeDecl(type_decl)
        {
            assert(is_a_ptr_of<NameListNode>(name_list));
            assert(is_a_ptr_of<SimpleTypeDeclNode>(type_decl));
        }
    };

    class RecordTypeNode : public AbstractTypeNode  // TODO: check all children are FieldDeclNode
    {
    public:
        Type getTypeClass() const override
        {
            // TODO
            assert(false);
        }
    };

    class TypeDefNode : public AbstractTypeNode  // TODO: should it be designed like this?
    {
    public:
        TypeDefNode(const NodePtr &name, const NodePtr &type_decl)
        {
            this->add(name);
            this->add(type_decl);
            assert(is_a_ptr_of<NameNode>(name));
            assert(is_a_ptr_of<SimpleTypeDeclNode>(type_decl));
        }

        Type getTypeClass() const override
        {
            // TODO
            assert(false);
        }
    };

    class AliasTypeNode : public AbstractTypeNode
    {
    public:
        AliasTypeNode(const NodePtr &name_node)
                : _name(std::dynamic_pointer_cast<NameNode>(name_node)->name)
        {}

        const std::string &name() const noexcept
        {
            return this->_name;
        }

    private:
        std::string _name;
    };

    class EnumTypeNode : public AbstractTypeNode
    {
    };

    class RangeTypeNode : public AbstractTypeNode
    {
    public:
        const NodePtr &min, &max;

        RangeTypeNode(const NodePtr &min, const NodePtr &max)
                : min(min), max(max)
        {}
    };

    class ArrayTypeNode : public AbstractTypeNode
    {
    };
}

#endif //NAIVE_PASCAL_COMPILER_TYPE_NODES_HPP
