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
            case Type::BOOLEAN: return context.builder.getInt1Ty();
            case Type::INTEGER: return context.builder.getInt32Ty();
            case Type::REAL: return context.builder.getDoubleTy();
            case Type::CHAR: return context.builder.getInt8Ty();
            case Type::VOID: return context.builder.getVoidTy();
            default: return nullptr;
        }
    }

    llvm::Type *ConstValueNode::get_llvm_type(CodegenContext &context) const
    {
        return llvm_type(type, context);
    }

    llvm::Type *TypeNode::get_llvm_type(CodegenContext &context) const
    {
        if (auto *simple_type = dynamic_cast<const SimpleTypeNode*>(this))
        { return llvm_type(simple_type->type, context); }
        else if (auto *alias = dynamic_cast<const AliasTypeNode*>(this))
        { return context.get_alias(alias->identifier->name); }
        else
        { throw CodegenException("unsupported type: " + to_string(type)); }
    }
}
