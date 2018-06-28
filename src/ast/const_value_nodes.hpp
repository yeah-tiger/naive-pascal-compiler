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

        llvm::Type *get_llvm_type(CodegenContext &context) const;

    protected:
        ConstValueNode() = default;

        bool should_have_children() const final
        { return false; }
    };

    class BooleanNode : public ConstValueNode
    {
    public:
        bool val;

        BooleanNode(bool val) : val(val)
        {
            type = Type::BOOLEAN;
        }

        llvm::Value *codegen(CodegenContext &context) override;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "Boolean", "value": ")"} +
                   (val ? "true" : "false") + "\"";
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

        llvm::Value *codegen(CodegenContext &context) override;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "Integer", "value": ")"} + std::to_string(val) + "\"";
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

        llvm::Value *codegen(CodegenContext &context) override;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "Real", "value": ")"} + std::to_string(val) + "\"";
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
            this->val = val[1];
            type = Type::CHAR;
        }

        llvm::Value *codegen(CodegenContext &context) override;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "Char", "value": ")"} + std::to_string(val) + "\"";
        }
    };

    class StringNode : public ConstValueNode
    {
    public:
        std::string val;

        StringNode(const char *val) : val(val)
        {
            this->val.erase(this->val.begin());
            this->val.pop_back();
            type = Type::STRING;
        }

        llvm::Value *codegen(CodegenContext &context) override;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "String", "value": ")"} + val + "\"";
        }
    };
}

#endif //NAIVE_PASCAL_COMPILER_CONST_VALUE_NODES_HPP
