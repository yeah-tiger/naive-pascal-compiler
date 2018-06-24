//
// Created by 孙耀珠 on 2018/6/24.
//

#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Constant.h>
#include <llvm/Support/Casting.h>
#include "utils/ast.hpp"
#include "utils/ast_utils.hpp"
#include "codegen/codegen_context.hpp"

namespace npc
{
    llvm::Value *ConstListNode::codegen(CodegenContext &context)
    {
        for (auto &child : children()) child->codegen(context);
        return nullptr;
    }

    llvm::Value *ConstDeclNode::codegen(CodegenContext &context)
    {
        if (context.is_subroutine)
        {
            auto *local = context.builder.CreateAlloca(value->get_llvm_type(context));
            context.set_local(name->name, local);
            context.builder.CreateStore(value->codegen(context), local);
            return local;
        }
        else
        {
            if (is_a_ptr_of<StringNode>(value))
            {
                return value->codegen(context);
            }
            auto *constant = llvm::cast<llvm::Constant>(value->codegen(context));
            return new llvm::GlobalVariable(*context.module, value->get_llvm_type(context), true,
                    llvm::GlobalVariable::ExternalLinkage, constant, name->name);
        }
    }

    llvm::Value *VarListNode::codegen(CodegenContext &context)
    {
        for (auto &child : children()) child->codegen(context);
        return nullptr;
    }

    llvm::Value *VarDeclNode::codegen(CodegenContext &context)
    {
        if (context.is_subroutine)
        {
            auto *local = context.builder.CreateAlloca(type->get_llvm_type(context));
            context.set_local(name->name, local);
            return local;
        }
        else
        {
            return new llvm::GlobalVariable(*context.module, type->get_llvm_type(context), false,
                    llvm::GlobalVariable::ExternalLinkage, type->get_default_value(context), name->name);
        }
    }
}
