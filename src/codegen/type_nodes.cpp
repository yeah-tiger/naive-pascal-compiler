//
// Created by 孙耀珠 on 2018/6/24.
//

#include <llvm/IR/Type.h>
#include <llvm/IR/Constants.h>
#include "utils/ast.hpp"
#include "codegen/codegen_context.hpp"

namespace npc
{
    static llvm::Type *llvm_type(Type type, CodegenContext &context)
    {
        switch (type)
        {
        case Type::BOOLEAN:
            return context.builder.getTrue()->getType();
        case Type::INTEGER:
            return context.builder.getInt32Ty();
        case Type::REAL:
            return context.builder.getDoubleTy();
        case Type::CHAR:
            return context.builder.getInt8Ty();
        case Type::STRING:
            return llvm::ArrayType::get(context.builder.getInt8Ty(), 256);
        default:
            assert(false);
            return nullptr;
        }
    }

    static llvm::Constant *llvm_constant(Type type, CodegenContext &context)
    {
        switch (type)
        {
            case Type::BOOLEAN:
                return context.builder.getFalse();
            case Type::INTEGER:
                return context.builder.getInt32(0);
            case Type::REAL:
                return llvm::ConstantFP::get(context.builder.getDoubleTy(), 0.0);
            case Type::CHAR:
                return context.builder.getInt8(0);
            default:
                assert(false);
                return nullptr;
        }
    }

    llvm::Type *ConstValueNode::get_llvm_type(CodegenContext &context) const
    {
        return llvm_type(type, context);
    }


    llvm::Type *TypeNode::get_llvm_type(CodegenContext &context) const
    {
        if (auto *simple_type = dynamic_cast<const SimpleTypeNode*>(this))
        {
            return llvm_type(simple_type->type, context);
        }
        else
        {
            assert(false);
            return nullptr;
        }
    }

    llvm::Constant *TypeNode::get_default_value(CodegenContext &context) const
    {
        return llvm_constant(type, context);
    }
}
