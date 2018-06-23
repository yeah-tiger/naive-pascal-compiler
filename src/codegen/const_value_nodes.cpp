//
// Created by 孙耀珠 on 2018/6/23.
//

#include <llvm/IR/Type.h>
#include <llvm/IR/Constants.h>
#include "utils/ast.hpp"
#include "codegen/codegen_context.hpp"

namespace npc
{
    llvm::Value *BooleanNode::codegen(CodegenContext &context)
    {

        return val ? context.builder.getTrue()
                   : context.builder.getFalse();
    }

    llvm::Value *IntegerNode::codegen(CodegenContext &context)
    {
        auto *type = context.builder.getInt32Ty();
        return llvm::ConstantInt::getSigned(type, val);
    }

    llvm::Value *RealNode::codegen(CodegenContext &context)
    {
        auto *type = context.builder.getDoubleTy();
        return llvm::ConstantFP::get(type, val);
    }

    llvm::Value *CharNode::codegen(CodegenContext &context)
    {
        return context.builder.getInt8(static_cast<uint8_t>(val));
    }

    llvm::Value *StringNode::codegen(CodegenContext &context)
    {
        return context.builder.CreateGlobalString(val);
    }
}
