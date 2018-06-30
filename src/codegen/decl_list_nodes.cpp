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
            auto success = context.set_local(name->name, local);
            if (!success) throw CodegenException("duplicate identifier in const section: " + name->name);
            context.builder.CreateStore(value->codegen(context), local);
            return local;
        }
        else
        {
            if (is_a_ptr_of<StringNode>(value))
            { return value->codegen(context); }
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
            auto success = context.set_local(name->name, local);
            if (!success) throw CodegenException("duplicate identifier in var section: " + name->name);
            return local;
        }
        else
        {
            auto *type = this->type->get_llvm_type(context);
            llvm::Constant *constant;
            if (type->isIntegerTy()) constant = llvm::ConstantInt::get(type, 0);
            else if (type->isDoubleTy()) constant = llvm::ConstantFP::get(context.builder.getDoubleTy(), 0.0);
            else throw CodegenException("unsupported type: " + to_string(this->type->type));
            return new llvm::GlobalVariable(*context.module, type, false, llvm::GlobalVariable::ExternalLinkage,
                                            constant, name->name);
        }
    }

    llvm::Value *TypeListNode::codegen(CodegenContext &context)
    {
        for (auto &child : children()) child->codegen(context);
        return nullptr;
    }

    llvm::Value *TypeDefNode::codegen(CodegenContext &context)
    {
        auto success = context.set_alias(name->name, type->get_llvm_type(context));
        if (!success) throw CodegenException("duplicate type alias: " + name->name);
        return nullptr;
    }
}
