//
// Created by 孙耀珠 on 2018/6/24.
//

#include "utils/ast.hpp"
#include "codegen/codegen_context.hpp"

namespace npc
{
    llvm::Value *IdentifierNode::get_ptr(CodegenContext &context)
    {
        auto *value = context.get_local(name);
        if (value == nullptr) value = context.module->getGlobalVariable(name);
        assert(value);
        return value;
    }

    llvm::Value *IdentifierNode::codegen(CodegenContext &context)
    {
        return context.builder.CreateLoad(get_ptr(context));
    }
}
