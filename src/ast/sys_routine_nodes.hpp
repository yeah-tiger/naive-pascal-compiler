//
// Created by 孙耀珠 on 2018/6/22.
//

#ifndef NAIVE_PASCAL_COMPILER_SYS_ROUTINE_NODES_HPP
#define NAIVE_PASCAL_COMPILER_SYS_ROUTINE_NODES_HPP

#include "ast/dummy_node.hpp"
#include "ast/decl_list_nodes.hpp"

namespace npc
{
    enum class SysRoutine
    {
        ABS, CHR, ODD, ORD, PRED, SQR, SQRT, SUCC,
        READ, READLN, WRITE, WRITELN
    };

    inline std::string to_string(SysRoutine routine)
    {
        std::map<SysRoutine, std::string> routine_to_string{
                {SysRoutine::ABS,     "abs"},
                {SysRoutine::CHR,     "chr"},
                {SysRoutine::ODD,     "odd"},
                {SysRoutine::ORD,     "ord"},
                {SysRoutine::PRED,    "pred"},
                {SysRoutine::SQR,     "sqr"},
                {SysRoutine::SQRT,    "sqrt"},
                {SysRoutine::SUCC,    "succ"},
                {SysRoutine::READ,    "read"},
                {SysRoutine::READLN,  "readln"},
                {SysRoutine::WRITE,   "write"},
                {SysRoutine::WRITELN, "writeln"}
        };
        // TODO: bound checking
        return routine_to_string[routine];
    }

    class SysRoutineNode : public DummyNode
    {
    public:
        SysRoutine routine;

        explicit SysRoutineNode(SysRoutine routine) : routine(routine)
        {}

    protected:
        bool should_have_children() const override
        { return false; }
    };

    class SysCallNode : public DummyNode
    {
    public:
        std::shared_ptr<SysRoutineNode> routine;
        std::shared_ptr<ArgListNode> args;

        SysCallNode(const NodePtr &routine, const NodePtr &args)
                : routine(cast_node<SysRoutineNode>(routine)), args(cast_node<ArgListNode>(args))
        {}

        llvm::Value *codegen(CodegenContext &context) override;

    protected:
        std::string json_head() const override
        {
            return std::string{R"("type": "SysCall", "identifier": ")"} + to_string(this->routine->routine) +
                   R"(", "args": )" + this->args->to_json();
        }

        bool should_have_children() const override
        { return false; }
    };
}

#endif //NAIVE_PASCAL_COMPILER_SYS_ROUTINE_NODES_HPP
