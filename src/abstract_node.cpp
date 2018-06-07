//
// Created by yzy on 6/7/18.
//

#include <iostream>
#include "abstract_node.hpp"

void npc::AbstractNode::describe() const
{
    std::clog << this->toString() << std::endl;
}
