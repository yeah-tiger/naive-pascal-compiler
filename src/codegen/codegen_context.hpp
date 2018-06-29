//
// Created by yzy on 6/7/18.
//

#ifndef NAIVE_PASCAL_COMPILER_CODEGEN_CONTEXT_H
#define NAIVE_PASCAL_COMPILER_CODEGEN_CONTEXT_H

#include <map>
#include <string>
#include <memory>
#include <exception>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>

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

        llvm::Type *get_alias(std::string key)
        {
            auto it = aliases.find(key);
            if (it != aliases.end()) return it->second;
            else return nullptr;
        }

        bool set_alias(std::string key, llvm::Type *value)
        {
            if (get_alias(key)) return false;
            aliases[key] = value;
            return true;
        }

    private:
        std::map<std::string, llvm::Value*> locals;
        std::map<std::string, llvm::Type*> aliases;
    };

    class CodegenException : public std::exception
    {
    public:
        explicit CodegenException(const std::string &description) : description(description)
        {}

        const char *what() const noexcept
        { return ("Codegen Error: " + description).c_str(); }

    private:
        std::string description;
    };
};

#endif //NAIVE_PASCAL_COMPILER_CODEGEN_CONTEXT_H
