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
    class AbstractNode
    {
    public:
        virtual ~AbstractNode() = default;

        virtual void codegen(CodegenContext &context) const = 0;

        void describe() const
        {
            std::clog << this->toString() << std::endl;
        }

        virtual std::string toString() const = 0;

        std::list<std::shared_ptr<AbstractNode>> &children()
        {
            return this->_children;
        }

        void add(const std::shared_ptr<AbstractNode> &node)
        {
            this->_children.push_back(node);
        }

        void add(std::shared_ptr<AbstractNode> &&node)
        {
            this->_children.push_back(node);
        }

    protected:
        std::list<std::shared_ptr<AbstractNode>> _children;
    };
}


#endif //NAIVE_PASCAL_COMPILER_AST_H
