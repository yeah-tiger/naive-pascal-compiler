//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_NAME_NODE_HPP
#define NAIVE_PASCAL_COMPILER_NAME_NODE_HPP

#include <string>
#include <utility>
#include "dummy_node.hpp"

namespace npc
{
    class NameNode : public DummyNode
    {
    public:
        NameNode(std::string _name)
                : name(std::move(_name))
        {}

        std::string name;
    };
}

#endif //NAIVE_PASCAL_COMPILER_NAME_NODE_HPP
