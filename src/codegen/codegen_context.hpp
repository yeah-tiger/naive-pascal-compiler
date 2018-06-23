//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_CODEGEN_CONTEXT_H
#define NAIVE_PASCAL_COMPILER_CODEGEN_CONTEXT_H

#include <memory>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

namespace npc
{
    class CodegenContext final
    {
    public:
        llvm::LLVMContext context;
        llvm::IRBuilder<> builder;
        std::unique_ptr<llvm::Module> module;

        CodegenContext(std::string module_id) : context(), builder(context)
        {
            module = std::make_unique<llvm::Module>(module_id, context);
        }

        CodegenContext() : CodegenContext("main")
        {}
    };
}

#endif //NAIVE_PASCAL_COMPILER_CODEGEN_CONTEXT_H
