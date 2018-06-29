//
// Created by 孙耀珠 on 2018/6/24.
//

#include <vector>
#include <llvm/Support/Casting.h>
#include "utils/ast.hpp"
#include "codegen/codegen_context.hpp"

namespace npc
{
    llvm::Value *CompoundStmtNode::codegen(CodegenContext &context)
    {
        for (auto &child : children()) child->codegen(context);
        return nullptr;
    }

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

    llvm::Value *IfStmtNode::codegen(CodegenContext &context)
    {
        auto *cond = expr->codegen(context);
        assert(cond->getType()->isIntegerTy(1));

        auto *func = context.builder.GetInsertBlock()->getParent();
        auto *then_block = llvm::BasicBlock::Create(context.module->getContext(), "then", func);
        auto *else_block = llvm::BasicBlock::Create(context.module->getContext(), "else");
        auto *cont_block = llvm::BasicBlock::Create(context.module->getContext(), "cont");
        context.builder.CreateCondBr(cond, then_block, else_block);

        context.builder.SetInsertPoint(then_block);
        stmt->codegen(context);
        context.builder.CreateBr(cont_block);

        func->getBasicBlockList().push_back(else_block);
        context.builder.SetInsertPoint(else_block);
        else_stmt->codegen(context);
        context.builder.CreateBr(cont_block);

        func->getBasicBlockList().push_back(cont_block);
        context.builder.SetInsertPoint(cont_block);
        return nullptr;
    }

    llvm::Value *CaseStmtNode::codegen(CodegenContext &context)
    {
        auto *value = expr->codegen(context);
        assert(value->getType()->isIntegerTy());
        auto *func = context.builder.GetInsertBlock()->getParent();
        auto *cont = llvm::BasicBlock::Create(context.module->getContext(), "cont");
        auto *switch_inst = context.builder.CreateSwitch(value, cont, static_cast<unsigned int>(children().size()));
        for (auto &child : children())
        {
            auto branch = cast_node<CaseExprNode>(child);
            auto *constant = llvm::cast<llvm::ConstantInt>(branch->branch->codegen(context));
            auto *block = llvm::BasicBlock::Create(context.module->getContext(), "case", func);
            context.builder.SetInsertPoint(block);
            branch->stmt->codegen(context);
            context.builder.CreateBr(cont);
            switch_inst->addCase(constant, block);
        }
        func->getBasicBlockList().push_back(cont);
        context.builder.SetInsertPoint(cont);
        return nullptr;
    }

    llvm::Value *RepeatStmtNode::codegen(CodegenContext &context)
    {
        auto *func = context.builder.GetInsertBlock()->getParent();
        auto *block = llvm::BasicBlock::Create(context.module->getContext(), "repeat", func);
        context.builder.CreateBr(block);

        context.builder.SetInsertPoint(block);
        for (auto &child : children()) child->codegen(context);
        auto *cond = expr->codegen(context);
        auto *cont = llvm::BasicBlock::Create(context.module->getContext(), "cont", func);
        context.builder.CreateCondBr(cond, cont, block);

        context.builder.SetInsertPoint(cont);
        return nullptr;
    }

    llvm::Value *WhileStmtNode::codegen(CodegenContext &context)
    {
        auto *func = context.builder.GetInsertBlock()->getParent();
        auto *while_block = llvm::BasicBlock::Create(context.module->getContext(), "while", func);
        auto *loop_block = llvm::BasicBlock::Create(context.module->getContext(), "loop", func);
        auto *cont_block = llvm::BasicBlock::Create(context.module->getContext(), "cont");
        context.builder.CreateBr(while_block);

        context.builder.SetInsertPoint(while_block);
        auto *cond = expr->codegen(context);
        context.builder.CreateCondBr(cond, loop_block, cont_block);

        context.builder.SetInsertPoint(loop_block);
        stmt->codegen(context);
        context.builder.CreateBr(while_block);

        func->getBasicBlockList().push_back(cont_block);
        context.builder.SetInsertPoint(cont_block);
        return nullptr;
    }

    llvm::Value *ForStmtNode::codegen(CodegenContext &context)
    {
        auto init = make_node<AssignStmtNode>(identifier, start);
        auto upto = direction == DirectionEnum::TO;
        auto cond = make_node<BinopExprNode>(upto ? BinaryOperator::LE : BinaryOperator::GE, identifier, finish);
        auto iter = make_node<AssignStmtNode>(identifier,
                make_node<BinopExprNode>(upto ? BinaryOperator::ADD :BinaryOperator::SUB,
                                         identifier, make_node<IntegerNode>(1)));
        auto compound = make_node<CompoundStmtNode>();
        compound->add_child(stmt); compound->add_child(iter);
        auto while_stmt = make_node<WhileStmtNode>(cond, compound);

        init->codegen(context);
        while_stmt->codegen(context);
        return nullptr;
    }
}
