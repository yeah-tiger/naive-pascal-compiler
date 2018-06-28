//
// Created by 孙耀珠 on 2018/6/23.
//

#include <vector>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include "utils/ast.hpp"
#include "codegen/codegen_context.hpp"

namespace npc
{
    llvm::Value *ProgramNode::codegen(CodegenContext &context)
    {
        context.is_subroutine = false;
        head_list->const_list->codegen(context);
        head_list->type_list->codegen(context);
        head_list->var_list->codegen(context);
        context.is_subroutine = true;
        head_list->subroutine_list->codegen(context);
        context.is_subroutine = false;

        auto *func_type = llvm::FunctionType::get(context.builder.getInt32Ty(), false);
        auto *main_func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                                                "main", context.module.get());
        auto *block = llvm::BasicBlock::Create(context.module->getContext(), "entry", main_func);
        context.builder.SetInsertPoint(block);
        for (auto &stmt : children()) stmt->codegen(context);
        context.builder.CreateRet(context.builder.getInt32(0));
        return nullptr;
    }

    llvm::Value *SubroutineNode::codegen(CodegenContext &context)
    {
        std::vector<llvm::Type*> types;
        std::vector<std::string> names;
        for (auto child : params->children())
        {
            auto decl = cast_node<ParamDeclNode>(child);
            types.push_back(decl->type->get_llvm_type(context));
            names.push_back(decl->name->name);
        }
        auto *func_type = llvm::FunctionType::get(type->get_llvm_type(context), types, false);
        auto *func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage,
                                            name->name, context.module.get());
        auto *block = llvm::BasicBlock::Create(context.module->getContext(), "entry", func);
        context.builder.SetInsertPoint(block);

        context.reset_locals();
        auto index = 0;
        for (auto &arg : func->args())
        {
            auto *local = context.builder.CreateAlloca(arg.getType());
            context.set_local(names[index++], local);
            context.builder.CreateStore(&arg, local);
        }
        if (type->type != Type::VOID)
        {
            auto *ret = context.builder.CreateAlloca(type->get_llvm_type(context));
            context.set_local(name->name, ret);
        }

        head_list->codegen(context);
        for (auto &stmt : children()) stmt->codegen(context);

        if (type->type == Type::VOID)
        {
            context.builder.CreateRetVoid();
        }
        else
        {
            auto *local = context.get_local(name->name);
            auto *ret = context.builder.CreateLoad(local);
            context.builder.CreateRet(ret);
        }
        return nullptr;
    }

    llvm::Value *HeadListNode::codegen(CodegenContext &context)
    {
        const_list->codegen(context);
        type_list->codegen(context);
        var_list->codegen(context);
        subroutine_list->codegen(context);
        return nullptr;

    }

    llvm::Value *SubroutineListNode::codegen(CodegenContext &context)
    {
        for (auto &routine : children()) routine->codegen(context);
        return nullptr;
    }
}
