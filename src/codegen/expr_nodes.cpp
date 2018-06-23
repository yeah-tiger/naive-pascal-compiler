//
// Created by 孙耀珠 on 2018/6/24.
//

#include "utils/ast.hpp"
#include "codegen/codegen_context.hpp"

namespace npc
{
    // TODO: check the type of the operands
    // TODO: handle non-integer operands
    llvm::Value *BinopExprNode::codegen(CodegenContext &context) {
        llvm::CmpInst::Predicate cmp;
        switch (op)
        {
        case BinaryOperator::GT:
            cmp = llvm::CmpInst::ICMP_SGT;
            break;
        case BinaryOperator::GE:
            cmp = llvm::CmpInst::ICMP_SGE;
            break;
        case BinaryOperator::LT:
            cmp = llvm::CmpInst::ICMP_SLT;
            break;
        case BinaryOperator::LE:
            cmp = llvm::CmpInst::ICMP_SLE;
            break;
        case BinaryOperator::EQ:
            cmp = llvm::CmpInst::ICMP_EQ;
            break;
        case BinaryOperator::NE:
            cmp = llvm::CmpInst::ICMP_NE;
            break;
        default:
            cmp = llvm::CmpInst::FCMP_FALSE;
        }
        if (cmp != llvm::CmpInst::FCMP_FALSE)
        {
            return context.builder.CreateICmp(cmp, lhs->codegen(context), rhs->codegen(context));
        }

        llvm::Instruction::BinaryOps binop;
        switch (op)
        {
        case BinaryOperator::ADD:
            binop = llvm::Instruction::Add;
            break;
        case BinaryOperator::SUB:
            binop = llvm::Instruction::Sub;
            break;
        case BinaryOperator::MUL:
            binop = llvm::Instruction::Mul;
            break;
        case BinaryOperator::DIV:
            binop = llvm::Instruction::SDiv;
            break;
        case BinaryOperator::MOD:
            binop = llvm::Instruction::SRem;
            break;
        case BinaryOperator::AND:
            binop = llvm::Instruction::And;
            break;
        case BinaryOperator::OR:
            binop = llvm::Instruction::Or;
            break;
        case BinaryOperator::XOR:
            binop = llvm::Instruction::Xor;
            break;
        default:
            assert(false);
        }
        return context.builder.CreateBinOp(binop, lhs->codegen(context), rhs->codegen(context));
    }
}
