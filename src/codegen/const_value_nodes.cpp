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
        return val ? llvm::ConstantInt::getTrue(context.context)
                   : llvm::ConstantInt::getFalse(context.context);
    }

    llvm::Value *IntegerNode::codegen(CodegenContext &context)
    {
        auto type = llvm::Type::getInt32Ty(context.context);
        return llvm::ConstantInt::getSigned(type, val);
    }

    llvm::Value *RealNode::codegen(CodegenContext &context) {
        auto type = llvm::Type::getDoubleTy(context.context);
        return llvm::ConstantFP::get(type, val);
    }

    llvm::Value *CharNode::codegen(CodegenContext &context) {
        auto type = llvm::Type::getInt8Ty(context.context);
        return llvm::ConstantInt::get(type, val);
    }
}
