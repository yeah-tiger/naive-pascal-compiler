//
// Created by yzy on 6/10/18.
//

#ifndef NAIVE_PASCAL_COMPILER_PARAMETER_NODES_HPP
#define NAIVE_PASCAL_COMPILER_PARAMETER_NODES_HPP

#include "dummy_node.hpp"
#include "type_nodes.hpp"

namespace npc
{
    class ParametersNode : public DummyNode
    {
    };

    class ParameterNode : public DummyNode
    {
    public:
        ParameterNode(NodePtr &_namelist, NodePtr &_type)
                : namelist(cast_node<NameList>(_namelist)), type(cast_node<TypeDeclNode>(_type))
        {}
        std::shared_ptr<NameList> namelist;
        std::shared_ptr<TypeDeclNode> type;
    };
}

#endif //NAIVE_PASCAL_COMPILER_PARAMETER_NODES_HPP
