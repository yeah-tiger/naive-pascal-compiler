//
// Created by 孙耀珠 on 2018/6/24.
//

#include <llvm/IR/LegacyPassNameParser.h>
#include "utils/ast.hpp"
#include "codegen/codegen_context.hpp"

namespace npc
{
    llvm::Value *AssignStmtNode::codegen(CodegenContext &context)
    {
        auto id = cast_node<IdentifierNode>(lhs);
        context.builder.CreateStore(rhs->codegen(context), id->get_value(context));
        return nullptr;
    }

    llvm::Value *ProcStmtNode::codegen(CodegenContext &context)
    {
        proc_call->codegen(context);
        return nullptr;
    }
}
