//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_AST_UTILS_HPP
#define NAIVE_PASCAL_COMPILER_AST_UTILS_HPP

#include <memory>
#include <utility>

namespace npc
{
    /*
     * NOTE: DO NOT include abstract_node.hpp
     * due to cycle including issue.
     */
    class AbstractNode;

    inline std::shared_ptr<AbstractNode> wrap_node(AbstractNode *node)
    {
        return std::shared_ptr<AbstractNode>{node};
    }

    template<typename NodeType, typename...Args>
    std::shared_ptr<AbstractNode> make_node(Args&&...args)
    {
        auto *node = new NodeType(std::forward<Args&&>(args)...);
        return std::shared_ptr<AbstractNode>(node);
    };

    template<typename NodeType>
    bool is_a_ptr_of(const std::shared_ptr<AbstractNode> &ptr)
    {
        auto _p = ptr.get();
        return dynamic_cast<NodeType *>(_p) != nullptr;
    }

    using NodePtr = std::shared_ptr<AbstractNode>;
}

#endif //NAIVE_PASCAL_COMPILER_AST_UTILS_HPP
