//
// Created by 孙耀珠 on 2018/6/24.
//

#include <llvm/Support/Casting.h>
#include "utils/ast.hpp"
#include "codegen/codegen_context.hpp"

namespace npc
{
    llvm::Value *SysCallNode::codegen(CodegenContext &context)
    {
        if (routine->routine == SysRoutine::WRITE || routine->routine == SysRoutine::WRITELN)
        {
            auto *char_ptr = context.builder.getInt8Ty()->getPointerTo();
            auto *printf_type = llvm::FunctionType::get(context.builder.getInt32Ty(), char_ptr, true);
            auto *printf_func = context.module->getOrInsertFunction("printf", printf_type);
            for (auto &arg : args->children())
            {
                auto *value = arg->codegen(context);
                std::vector<llvm::Value*> args;
                if (value->getType()->isIntegerTy(8))
                { args.push_back(context.builder.CreateGlobalStringPtr("%c")); args.push_back(value); }
                else if (value->getType()->isIntegerTy())
                { args.push_back(context.builder.CreateGlobalStringPtr("%d")); args.push_back(value); }
                else if (value->getType()->isDoubleTy())
                { args.push_back(context.builder.CreateGlobalStringPtr("%f")); args.push_back(value); }
                // Pascal pointers are not supported, so this is an LLVM global string pointer.
                else if (value->getType()->isPointerTy())
                { args.push_back(value); }
                else
                { throw CodegenException("incompatible type in write(): expected char, integer, real"); }
                context.builder.CreateCall(printf_func, args);
            }
            if (routine->routine == SysRoutine::WRITELN)
            { context.builder.CreateCall(printf_func, context.builder.CreateGlobalStringPtr("\n")); }
            return nullptr;
        }
        else if (routine->routine == SysRoutine::READ || routine->routine == SysRoutine::READLN)
        {
            auto *char_ptr = context.builder.getInt8Ty()->getPointerTo();
            auto *scanf_type = llvm::FunctionType::get(context.builder.getInt32Ty(), char_ptr, true);
            auto *scanf_func = context.module->getOrInsertFunction("scanf", scanf_type);
            for (auto &arg : args->children())
            {
                auto ptr = cast_node<IdentifierNode>(arg)->get_ptr(context);
                std::vector<llvm::Value*> args;
                if (ptr->getType()->getPointerElementType()->isIntegerTy(8))
                { args.push_back(context.builder.CreateGlobalStringPtr("%c")); args.push_back(ptr); }
                else if (ptr->getType()->getPointerElementType()->isIntegerTy(32))
                { args.push_back(context.builder.CreateGlobalStringPtr("%d")); args.push_back(ptr); }
                else if (ptr->getType()->getPointerElementType()->isDoubleTy())
                { args.push_back(context.builder.CreateGlobalStringPtr("%lf")); args.push_back(ptr); }
                else
                { throw CodegenException("incompatible type in read(): expected char, integer, real"); }
                context.builder.CreateCall(scanf_func, args);
            }
            if (routine->routine == SysRoutine::READLN)
            { context.builder.CreateCall(scanf_func, context.builder.CreateGlobalStringPtr("%*[^\n]\n")); }
            return nullptr;
        }
        else if (routine->routine == SysRoutine::ABS)
        {
            if (args->children().size() != 1)
            { throw CodegenException("wrong number of arguments: abs()"); }
            auto *value = args->children().front()->codegen(context);
            if (value->getType()->isIntegerTy(32))
            {
                auto *int32_ty = context.builder.getInt32Ty();
                auto *abs_type = llvm::FunctionType::get(int32_ty, int32_ty, false);
                auto *abs_func = context.module->getOrInsertFunction("abs", abs_type);
                return context.builder.CreateCall(abs_func, value);
            }
            else if (value->getType()->isDoubleTy())
            {
                auto *double_ty = context.builder.getDoubleTy();
                auto *fabs_type = llvm::FunctionType::get(double_ty, double_ty, false);
                auto *fabs_func = context.module->getOrInsertFunction("fabs", fabs_type);
                return context.builder.CreateCall(fabs_func, value);
            }
            else
            {
                throw CodegenException("incompatible type in abs(): expected integer, real");
            }
        }
        else if (routine->routine == SysRoutine::SQRT)
        {
            if (args->children().size() != 1)
            { throw CodegenException("wrong number of arguments: sqrt()"); }
            auto *value = args->children().front()->codegen(context);
            auto *double_ty = context.builder.getDoubleTy();
            if (value->getType()->isIntegerTy(32))
            { value = context.builder.CreateSIToFP(value, double_ty); }
            else if (!value->getType()->isDoubleTy())
            { throw CodegenException("incompatible type in sqrt(): expected integer, real"); }
            auto *sqrt_type = llvm::FunctionType::get(double_ty, double_ty, false);
            auto *sqrt_func = context.module->getOrInsertFunction("sqrt", sqrt_type);
            return context.builder.CreateCall(sqrt_func, value);
        }
        else if (routine->routine == SysRoutine::CHR)
        {
            if (args->children().size() != 1)
            { throw CodegenException("wrong number of arguments: chr()"); }
            auto *value = args->children().front()->codegen(context);
            if (!value->getType()->isIntegerTy(32))
            { throw CodegenException("incompatible type in chr(): expected integer"); }
            return context.builder.CreateTrunc(value, context.builder.getInt8Ty());
        }
        else if (routine->routine == SysRoutine::ORD)
        {
            if (args->children().size() != 1)
            { throw CodegenException("wrong number of arguments: ord()"); }
            auto *value = args->children().front()->codegen(context);
            if (!value->getType()->isIntegerTy(8))
            { throw CodegenException("incompatible type in ord(): expected char"); }
            return context.builder.CreateZExt(value, context.builder.getInt32Ty());
        }
        else if (routine->routine == SysRoutine::PRED)
        {
            if (args->children().size() != 1)
            { throw CodegenException("wrong number of arguments: pred()"); }
            auto *value = args->children().front()->codegen(context);
            if (!value->getType()->isIntegerTy(8))
            { throw CodegenException("incompatible type in pred(): expected char"); }
            return context.builder.CreateBinOp(llvm::Instruction::Sub, value, context.builder.getInt8(1));
        }
        else if (routine->routine == SysRoutine::SUCC)
        {
            if (args->children().size() != 1)
            { throw CodegenException("wrong number of arguments: succ()"); }
            auto *value = args->children().front()->codegen(context);
            if (!value->getType()->isIntegerTy(8))
            { throw CodegenException("incompatible type in succ(): expected char"); }
            return context.builder.CreateBinOp(llvm::Instruction::Add, value, context.builder.getInt8(1));
        }
        else
        {
            throw CodegenException("unsupported built-in routine: " + to_string(routine->routine));
        }
    }
}
