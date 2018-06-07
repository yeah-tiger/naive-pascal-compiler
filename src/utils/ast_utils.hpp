//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_AST_UTILS_HPP
#define NAIVE_PASCAL_COMPILER_AST_UTILS_HPP

#include <memory>
#include <utility>
#include "../ast/abstract_node.hpp"

namespace npc
{
    std::shared_ptr<AbstractNode> make_node(AbstractNode *node)
    {
        return std::shared_ptr<AbstractNode>{node};
    }

    template<typename NodeType, typename...Args>
    std::shared_ptr<AbstractNode> make_node(Args...args)
    {
        auto *node = new NodeType{std::forward(args)...};
        return std::shared_ptr<AbstractNode>{node};
    };

    template<typename NodeType>
    bool is_a_ptr_of(const std::shared_ptr<AbstractNode> &ptr)
    {
        auto _p = ptr.get();
        return !(!dynamic_cast<NodeType>(_p) != nullptr);
    }
}

#endif //NAIVE_PASCAL_COMPILER_AST_UTILS_HPP
