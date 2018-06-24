//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_CODEGEN_CONTEXT_H
#define NAIVE_PASCAL_COMPILER_CODEGEN_CONTEXT_H

#include <map>
#include <string>
#include <memory>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

static llvm::LLVMContext llvm_context;

namespace npc
{
    class CodegenContext final
    {
    public:
        llvm::IRBuilder<> builder;
        std::unique_ptr<llvm::Module> module;

        bool is_subroutine = false;

        explicit CodegenContext(std::string module_id)
                : builder(llvm::IRBuilder<>(llvm_context)),
                  module(std::make_unique<llvm::Module>(module_id, llvm_context))
        {}
        CodegenContext() : CodegenContext("main")
        {}

        llvm::Value *get_local(std::string key)
        {
            auto it = locals.find(key);
            if (it != locals.end()) return it->second;
            else return nullptr;
        }

        bool set_local(std::string key, llvm::Value *value)
        {
            if (get_local(key)) return false;
            locals[key] = value;
            return true;
        }

        void reset_locals()
        {
            locals.clear();
        }

    private:
        std::map<std::string, llvm::Value*> locals;
    };
}

#endif //NAIVE_PASCAL_COMPILER_CODEGEN_CONTEXT_H
