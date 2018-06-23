//
// Created by yzy on 6/9/18.
//

#ifndef NAIVE_PASCAL_COMPILER_CONST_VALUE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_CONST_VALUE_NODES_HPP

#include <sstream>
#include "dummy_node.hpp"
#include "expr_nodes.hpp"
#include "type_nodes.hpp"

namespace npc
{
    class ConstValueNode : public ExprNode
    {
    public:
        Type type = Type::UNDEFINED;
    };

    class BooleanNode : public ConstValueNode
    {
    public:
        bool val;

        BooleanNode(bool val) : val(val)
        {
            type = Type::BOOLEAN;
        }

    protected:
        std::string jsonHead() const override
        {
            return std::string{R"EOF("type": "boolean", "value": ")EOF"} + (val ? "true" : "false") +
                   std::string{"\","};
        }
    };

    class IntegerNode : public ConstValueNode
    {
    public:
        int val;

        IntegerNode(int val) : val(val)
        {
            type = Type::INTEGER;
        }

        IntegerNode(const char *val)
        {
            std::stringstream ss;
            ss << val;
            ss >> this->val;
            type = Type::INTEGER;
        }

    protected:
        std::string jsonHead() const override
        {
            return std::string{R"EOF("type": "integer", "value": ")EOF"} + std::to_string(val) +
                   std::string{"\","};
        }
    };

    class RealNode : public ConstValueNode
    {
    public:
        double val;

        RealNode(double val) : val(val)
        {
            type = Type::REAL;
        }

        RealNode(const char *val)
        {
            std::stringstream ss;
            ss << val;
            ss >> this->val;
            type = Type::REAL;
        }

    protected:
        std::string jsonHead() const override
        {
            return std::string{R"EOF("type": "real", "value": ")EOF"} + std::to_string(val) +
                   std::string{"\","};
        }
    };

    class CharNode : public ConstValueNode
    {
    public:
        char val;

        CharNode(const char val) : val(val)
        {
            type = Type::CHAR;
        }

        CharNode(const char *val)
        {
            this->val = *val;
            type = Type::CHAR;
        }

    protected:
        std::string jsonHead() const override
        {
            return std::string{R"EOF("type": "char", "value": ")EOF"} + std::to_string(val) +
                   std::string{"\","};
        }
    };

    class StringNode : public ConstValueNode
    {
    public:
        std::string val;

        StringNode(const char *val) : val(val)
        {
            type = Type::STRING;
        }

    protected:
        std::string jsonHead() const override
        {
            return std::string{R"EOF("type": "string", "value": ")EOF"} + val +
                   std::string{"\","};
        }
    };
}

#endif //NAIVE_PASCAL_COMPILER_CONST_VALUE_NODES_HPP
