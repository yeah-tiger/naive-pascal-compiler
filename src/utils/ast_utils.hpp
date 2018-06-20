//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_AST_UTILS_HPP
#define NAIVE_PASCAL_COMPILER_AST_UTILS_HPP

#include <memory>
#include <utility>
#include <type_traits>
#include <cassert>

namespace npc
{
    /*
     * NOTE: DO NOT include abstract_node.hpp
     * due to cycle including issue.
     */
    class AbstractNode;

    template<typename NodeType>
    bool is_a_ptr_of(const std::shared_ptr<AbstractNode> &ptr)
    {
        auto _p = ptr.get();
        return dynamic_cast<NodeType *>(_p) != nullptr;
    }

    inline std::shared_ptr<AbstractNode> wrap_node(AbstractNode *node)
    {
        return std::shared_ptr<AbstractNode>{node};
    }

    template<typename TNode>
    typename std::enable_if<std::is_base_of<AbstractNode, TNode>::value, std::shared_ptr<TNode>>::type
    cast_node(const std::shared_ptr<AbstractNode> &node)
    {
        assert(is_a_ptr_of<TNode>(node));
        return std::dynamic_pointer_cast<TNode>(node);
    }

    template<typename NodeType, typename...Args>
    std::shared_ptr<AbstractNode> make_node(Args &&...args)
    {
        return std::static_pointer_cast<AbstractNode>(
                std::make_shared<NodeType>(std::forward<Args>(args)...)
        );
    };

    using NodePtr = std::shared_ptr<AbstractNode>;
}

#endif //NAIVE_PASCAL_COMPILER_AST_UTILS_HPP
