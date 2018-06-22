//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_CONST_VALUE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_CONST_VALUE_NODES_HPP

#include <sstream>
#include "dummy_node.hpp"
#include "expr_nodes.hpp"

namespace npc
{
    class ConstValueNode : public ExprNode
    {
    public:
        // TODO: remove negative()
        virtual NodePtr negative() const = 0;
    };

    class IntegerNode : public ConstValueNode
    {
    public:
        int64_t val;

        IntegerNode(int64_t v)
        { val = v; }

        IntegerNode(const char *v)
        {
            std::stringstream ss;
            ss << v;
            ss >> this->val;
        }

        NodePtr negative() const override
        {
            return make_node<IntegerNode>(-val);
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

        RealNode(const double v) : val(v)
        {}

        NodePtr negative() const override
        {
            return make_node<RealNode>(-val);
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

        CharNode(const char v) : val(v)
        {}

        NodePtr negative() const override
        {
            return make_node<CharNode>(-val);
        }
    };

    class StringNode : public ConstValueNode
    {
    public:
        std::string val;

        StringNode(const char *sz)
                : val(std::string(sz))
        {}

        NodePtr negative() const override
        {
            assert(false);
            return make_node<DummyNode>();
        }
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

        NodePtr negative() const override
        {
            assert(false);
            return make_node<DummyNode>();
        }
    };
}

#endif //NAIVE_PASCAL_COMPILER_CONST_VALUE_NODES_HPP
