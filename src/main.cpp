//
// Created by yzy on 6/7/18.
// Modified by 孙耀珠 on 2018/6/29.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <system_error>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>
#include "utils/ast.hpp"
#include "codegen/codegen_context.hpp"
#include "y.tab.h"

using namespace std;
using namespace llvm;
using namespace npc;

extern YYSTYPE program;

void emit_target(raw_fd_ostream &dest, TargetMachine::CodeGenFileType type, Module &module)
{
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();

    auto target_triple = sys::getDefaultTargetTriple();
    module.setTargetTriple(target_triple);

    string error;
    auto target = TargetRegistry::lookupTarget(target_triple, error);
    if (!target)
    { errs() << error; exit(1); }

    auto cpu = "generic";
    auto features = "";
    TargetOptions opt;
    auto rm = Optional<Reloc::Model>();
    auto target_machine = target->createTargetMachine(target_triple, cpu, features, opt, rm);
    module.setDataLayout(target_machine->createDataLayout());

    legacy::PassManager pass;
    if (target_machine->addPassesToEmitFile(pass, dest, type))
    { errs() << "The target machine cannot emit an object file"; exit(1); }

    pass.run(module);
    dest.flush();
}

int main(int argc, char *argv[])
{
    enum class Target
    { UNDEFINED, LLVM, ASM, OBJ };
    Target target = Target::UNDEFINED;
    char *input = nullptr;
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-emit-llvm") == 0) target = Target::LLVM;
        else if (strcmp(argv[i], "-S") == 0) target = Target::ASM;
        else if (strcmp(argv[i], "-c") == 0) target = Target::OBJ;
        else input = argv[i];
    }
    if (target == Target::UNDEFINED || input == nullptr)
    {
        puts("USAGE: npc <option> <input.pas>");
        puts("OPTION:");
        puts("  -emit-llvm    Emit LLVM assembly code (.ll)");
        puts("  -S            Emit assembly code (.s)");
        puts("  -c            Emit object code (.o)");
        exit(1);
    }
    freopen(input, "r", stdin);

    yyparse();
    CodegenContext context;
    program->codegen(context);

    string output = input;
    output.erase(output.rfind('.'));
    switch (target)
    {
        case Target::LLVM: output.append(".ll"); break;
        case Target::ASM:  output.append(".s");  break;
        case Target::OBJ:  output.append(".o");  break;
        default: break;
    }
    error_code ec;
    raw_fd_ostream fd(output, ec, sys::fs::F_None);
    if (ec)
    { errs() << "Could not open file: " << ec.message(); exit(1); }

    switch (target)
    {
        case Target::LLVM: context.module->print(fd, nullptr); break;
        case Target::ASM: emit_target(fd, TargetMachine::CGFT_AssemblyFile, *context.module); break;
        case Target::OBJ: emit_target(fd, TargetMachine::CGFT_ObjectFile, *context.module); break;
        default: break;
    }
    return 0;
}
