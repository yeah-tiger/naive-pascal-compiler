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
        std::vector<llvm::Value*> values;
        for (auto &arg : args->children()) values.push_back(arg->codegen(context));
        return context.builder.CreateCall(func, values);
    }
}
