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
        assert(routine->routine == SysRoutine::WRITELN);
        auto *char_ptr = context.builder.getInt8Ty()->getPointerTo();
        auto *printf_type = llvm::FunctionType::get(context.builder.getInt32Ty(), char_ptr, true);
        auto *printf_func = context.module->getOrInsertFunction("printf", printf_type);
        for (auto &arg : args->children())
        {
            auto *value = arg->codegen(context);
            std::vector<llvm::Value*> args;
            if (value->getType()->isIntegerTy())
            {
                args.push_back(context.builder.CreateGlobalStringPtr("%d"));
                args.push_back(value);
            }
            else if (value->getType()->isDoubleTy())
            {
                args.push_back(context.builder.CreateGlobalStringPtr("%f"));
                args.push_back(value);
            }
            else
            {
                // FIXME: a hack for printing string literals
                auto *gv = llvm::cast<llvm::GlobalVariable>(value);
                auto *zero = context.builder.getInt32(0);
                llvm::Value *zeros[] = { zero, zero };
                auto *ptr = context.builder.CreateInBoundsGEP(gv->getValueType(), gv, zeros);
                args.push_back(ptr);
            }
            context.builder.CreateCall(printf_func, args);
        }
        context.builder.CreateCall(printf_func, context.builder.CreateGlobalStringPtr("\n"));
        return nullptr;
    }
}
