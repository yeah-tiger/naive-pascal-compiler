//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_AST_H
#define NAIVE_PASCAL_COMPILER_AST_H

#include <string>
#include <memory>
#include <list>
#include "codegen_context.hpp"

namespace npc
{
    class AbstractNode
    {
    public:
        virtual ~AbstractNode() = default;

        virtual void codegen(CodegenContext &context) const = 0;

        void describe() const;

        virtual std::string toString() const = 0;

        std::list<std::shared_ptr<AbstractNode>> &children()
        {
            return this->_children;
        }

    private:
        std::list<std::shared_ptr<AbstractNode>> _children;
    };
}


#endif //NAIVE_PASCAL_COMPILER_AST_H
