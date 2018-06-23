//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_AST_H
#define NAIVE_PASCAL_COMPILER_AST_H

#include <string>
#include <memory>
#include <list>
#include <iostream>
#include <sstream>

#include "codegen_context.hpp"
#include "../utils/ast_utils.hpp"

namespace npc
{
    class AbstractNode : public std::enable_shared_from_this<AbstractNode>
    {
    public:
        // NOLINT
        virtual ~AbstractNode() noexcept = default;  // the base template class has a trivial destructor.

        virtual void codegen(CodegenContext &context) const = 0;

        void describe() const
        {
            std::clog << this->toString() << std::endl;
        }

        virtual std::string toString() const = 0;

        std::string toJson() const
        {
            std::stringstream ret{"{"};
            ret << this->jsonHead();
            if (this->should_have_children())
            {
                ret << "[";
                auto &children = this->_children;
                for (auto &node : children)
                {
                    ret << node->toJson();
                    if (node != *children.end())
                    {
                        ret << ",";
                    }
                }
                ret << "]";
            }
            ret << "}";
            return ret.str();
        }

        std::list<std::shared_ptr<AbstractNode>> &children() noexcept
        {
            assert(this->should_have_children());
            return this->_children;
        }

        auto &parent() noexcept
        {
            return this->_parent;
        }

        void add_child(const std::shared_ptr<AbstractNode> &node)
        {
            this->_children.push_back(node);
            node->parent() = this->shared_from_this();
        }

        void add_child(std::shared_ptr<AbstractNode> &&node)
        {
            this->_children.push_back(node);
            node->parent() = this->shared_from_this();
        }

        void merge_children(const std::list<std::shared_ptr<AbstractNode>> &children)
        {
            for (const auto &e : children)
            {
                this->add_child(e);
            }
        }

        void lift_children(const std::shared_ptr<AbstractNode> &node)
        {
            this->merge_children(node->children());
        }

        void move_children(const std::shared_ptr<AbstractNode> &node)
        {
            this->children() = std::move(node->children());
        }

    protected:
        std::list<std::shared_ptr<AbstractNode>> _children;
        std::weak_ptr<AbstractNode> _parent;

        virtual const bool should_have_children() const
        { return true; }

        virtual std::string jsonHead() const = 0;
    };
}


#endif //NAIVE_PASCAL_COMPILER_AST_H
