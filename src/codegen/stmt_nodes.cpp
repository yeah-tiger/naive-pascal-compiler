//
// Created by 孙耀珠 on 2018/6/24.
//

#include "utils/ast.hpp"
#include "codegen/codegen_context.hpp"

namespace npc
{
    llvm::Value *AssignStmtNode::codegen(CodegenContext &context)
    {
        auto *lhs = cast_node<IdentifierNode>(this->lhs)->get_ptr(context);
        auto *rhs = this->rhs->codegen(context);
        if (lhs->getType()->getPointerElementType()->isDoubleTy() && rhs->getType()->isIntegerTy(32))
        {
            rhs = context.builder.CreateSIToFP(rhs, context.builder.getDoubleTy());
        }
        // TODO: check if types are compatible
        context.builder.CreateStore(rhs, lhs);
        return nullptr;
    }

    llvm::Value *ProcStmtNode::codegen(CodegenContext &context)
    {
        proc_call->codegen(context);
        return nullptr;
    }
}
