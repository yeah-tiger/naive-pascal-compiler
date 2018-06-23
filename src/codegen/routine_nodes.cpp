//
// Created by 孙耀珠 on 2018/6/23.
//

#include "utils/ast.hpp"
#include "codegen/codegen_context.hpp"

namespace npc
{
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
