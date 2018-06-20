//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_AST_H
#define NAIVE_PASCAL_COMPILER_AST_H

#include <string>
#include <memory>
#include <list>
#include <iostream>

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

        std::list<std::shared_ptr<AbstractNode>> &children() noexcept
        {
            return this->_children;
        }

        auto &parent() noexcept
        {
            return this->_parent;
        }

        void add(const std::shared_ptr<AbstractNode> &node)
        {
            this->_children.push_back(node);
            node->parent() = this;
        }

        void add(std::shared_ptr<AbstractNode> &&node)
        {
            this->_children.push_back(node);
            node->parent() = this;
        }

        void merge_children(const std::list<std::shared_ptr<AbstractNode>> &children)
        {
            for (const auto &e : children)
            {
                this->add(e);
            }
        }

    protected:
        std::list<std::shared_ptr<AbstractNode>> _children;

        AbstractNode *_parent;
    };
}


#endif //NAIVE_PASCAL_COMPILER_AST_H
