//
// Created by 孙耀珠 on 2018/6/24.
//

#include "utils/ast.hpp"
#include "codegen/codegen_context.hpp"

namespace npc
{
    llvm::Value *BinopExprNode::codegen(CodegenContext &context) {
        auto *lhs = this->lhs->codegen(context);
        auto *rhs = this->rhs->codegen(context);
        if (lhs->getType()->isIntegerTy(1) && rhs->getType()->isIntegerTy(1))
        {
            llvm::CmpInst::Predicate cmp;
            switch (op)
            {
                case BinaryOperator::GT: cmp = llvm::CmpInst::ICMP_SGT; break;
                case BinaryOperator::GE: cmp = llvm::CmpInst::ICMP_SGE; break;
                case BinaryOperator::LT: cmp = llvm::CmpInst::ICMP_SLT; break;
                case BinaryOperator::LE: cmp = llvm::CmpInst::ICMP_SLE; break;
                case BinaryOperator::EQ: cmp = llvm::CmpInst::ICMP_EQ; break;
                case BinaryOperator::NE: cmp = llvm::CmpInst::ICMP_NE; break;
                default: cmp = llvm::CmpInst::FCMP_FALSE;
            }
            if (cmp != llvm::CmpInst::FCMP_FALSE) return context.builder.CreateICmp(cmp, lhs, rhs);
            llvm::Instruction::BinaryOps binop;
            switch (op)
            {
                case BinaryOperator::AND: binop = llvm::Instruction::And; break;
                case BinaryOperator::OR:  binop = llvm::Instruction::Or; break;
                case BinaryOperator::XOR: binop = llvm::Instruction::Xor; break;
                default: assert(false);
            }
            return context.builder.CreateBinOp(binop, lhs, rhs);
        }
        else if (lhs->getType()->isIntegerTy(32) && rhs->getType()->isIntegerTy(32))
        {
            llvm::CmpInst::Predicate cmp;
            switch (op)
            {
                case BinaryOperator::GT: cmp = llvm::CmpInst::ICMP_SGT; break;
                case BinaryOperator::GE: cmp = llvm::CmpInst::ICMP_SGE; break;
                case BinaryOperator::LT: cmp = llvm::CmpInst::ICMP_SLT; break;
                case BinaryOperator::LE: cmp = llvm::CmpInst::ICMP_SLE; break;
                case BinaryOperator::EQ: cmp = llvm::CmpInst::ICMP_EQ; break;
                case BinaryOperator::NE: cmp = llvm::CmpInst::ICMP_NE; break;
                default: cmp = llvm::CmpInst::FCMP_FALSE;
            }
            if (cmp != llvm::CmpInst::FCMP_FALSE) return context.builder.CreateICmp(cmp, lhs, rhs);
            llvm::Instruction::BinaryOps binop;
            switch (op)
            {
                case BinaryOperator::ADD: binop = llvm::Instruction::Add; break;
                case BinaryOperator::SUB: binop = llvm::Instruction::Sub; break;
                case BinaryOperator::MUL: binop = llvm::Instruction::Mul; break;
                case BinaryOperator::TRUEDIV: binop = llvm::Instruction::FDiv; break;
                case BinaryOperator::DIV: binop = llvm::Instruction::SDiv; break;
                case BinaryOperator::MOD: binop = llvm::Instruction::SRem; break;
                case BinaryOperator::AND: binop = llvm::Instruction::And; break;
                case BinaryOperator::OR: binop = llvm::Instruction::Or; break;
                case BinaryOperator::XOR: binop = llvm::Instruction::Xor; break;
                default: assert(false);
            }
            return context.builder.CreateBinOp(binop, lhs, rhs);
        }
        else if (lhs->getType()->isDoubleTy() || rhs->getType()->isDoubleTy())
        {
            if (lhs->getType()->isIntegerTy(32)) lhs = context.builder.CreateSIToFP(lhs, context.builder.getDoubleTy());
            if (rhs->getType()->isIntegerTy(32)) rhs = context.builder.CreateSIToFP(rhs, context.builder.getDoubleTy());
            llvm::CmpInst::Predicate cmp;
            switch (op)
            {
                case BinaryOperator::GT: cmp = llvm::CmpInst::FCMP_OGT; break;
                case BinaryOperator::GE: cmp = llvm::CmpInst::FCMP_OGE; break;
                case BinaryOperator::LT: cmp = llvm::CmpInst::FCMP_OLT; break;
                case BinaryOperator::LE: cmp = llvm::CmpInst::FCMP_OLE; break;
                case BinaryOperator::EQ: cmp = llvm::CmpInst::FCMP_OEQ; break;
                case BinaryOperator::NE: cmp = llvm::CmpInst::FCMP_ONE; break;
                default: cmp = llvm::CmpInst::FCMP_FALSE;
            }
            if (cmp != llvm::CmpInst::FCMP_FALSE) return context.builder.CreateFCmp(cmp, lhs, rhs);
            llvm::Instruction::BinaryOps binop;
            switch (op)
            {
                case BinaryOperator::ADD: binop = llvm::Instruction::FAdd; break;
                case BinaryOperator::SUB: binop = llvm::Instruction::FSub; break;
                case BinaryOperator::MUL: binop = llvm::Instruction::FMul; break;
                case BinaryOperator::TRUEDIV: binop = llvm::Instruction::FDiv; break;
                default: assert(false);
            }
            return context.builder.CreateBinOp(binop, lhs, rhs);
        }
        else if (lhs->getType()->isIntegerTy(8) && rhs->getType()->isIntegerTy(8))
        {
            llvm::CmpInst::Predicate cmp;
            switch (op)
            {
                case BinaryOperator::GT: cmp = llvm::CmpInst::ICMP_SGT; break;
                case BinaryOperator::GE: cmp = llvm::CmpInst::ICMP_SGE; break;
                case BinaryOperator::LT: cmp = llvm::CmpInst::ICMP_SLT; break;
                case BinaryOperator::LE: cmp = llvm::CmpInst::ICMP_SLE; break;
                case BinaryOperator::EQ: cmp = llvm::CmpInst::ICMP_EQ; break;
                case BinaryOperator::NE: cmp = llvm::CmpInst::ICMP_NE; break;
                default: assert(false);
            }
            return context.builder.CreateICmp(cmp, lhs, rhs);
        }
        else
        {
            assert(false);
            return nullptr;
        }
    }

    llvm::Value *FuncExprNode::codegen(CodegenContext &context)
    {
        return func_call->codegen(context);
    }
}
