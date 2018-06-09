//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_CONST_VALUE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_CONST_VALUE_NODES_HPP

#include <sstream>
#include "dummy_node.hpp"

namespace npc
{
    class ConstValueNode : public DummyNode
    {
    };

    class IntegerNode : public ConstValueNode
    {
    public:
        int64_t val;

        IntegerNode(const char *v)
        {
            std::stringstream ss;
            ss << v;
            ss >> this->val;
        }
    };

    class RealNode : public ConstValueNode
    {
    public:
        double val;

        RealNode(const char *v)
        {
            std::stringstream ss;
            ss << v;
            ss >> this->val;
        }
    };

    class CharNode : public ConstValueNode
    {
    public:
        char val;

        CharNode(const char *v)
        {
            std::stringstream ss;
            ss << v;
            ss >> this->val;
        }
    };

    class StringNode : public ConstValueNode
    {
    public:
        std::string val;

        StringNode(const char *sz)
                : val(std::string(sz))
        {}
    };

    enum class SysConEnum
    {
        TRUE,
        FALSE,
        MAXINT
    };

    class SysConNode : public ConstValueNode
    {
    public:
        SysConEnum val;

        SysConNode(SysConEnum v)
                : val(v)
        {}
    };
}

#endif //NAIVE_PASCAL_COMPILER_CONST_VALUE_NODES_HPP
