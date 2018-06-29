//
// Created by 孙耀珠 on 2018/6/24.
//

#include <vector>
#include "utils/ast.hpp"
#include "codegen/codegen_context.hpp"

namespace npc
{
    llvm::Value *RoutineCallNode::codegen(CodegenContext &context)
    {
        auto *func = context.module->getFunction(identifier->name);
        if (func->arg_size() != args->children().size())
        { throw CodegenException("wrong number of arguments: " + identifier->name + "()"); }
        std::vector<llvm::Value*> values;
        // TODO: check type compatibility between arguments and parameters
        for (auto &arg : args->children()) values.push_back(arg->codegen(context));
        return context.builder.CreateCall(func, values);
    }
}
