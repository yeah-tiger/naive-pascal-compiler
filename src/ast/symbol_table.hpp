//
// Created by yzy on 6/18/18.
//

#ifndef NAIVE_PASCAL_COMPILER_SYMBOL_TABLE_HPP
#define NAIVE_PASCAL_COMPILER_SYMBOL_TABLE_HPP

#include <string>
#include <unordered_map>
#include "type_nodes.hpp"

namespace npc
{
    // TODO: modify fields of variable.
    class Variable
    {
    public:
        std::string name;
        Type type;
    };

    using SymbolTable = std::unordered_map<std::string, Variable>;

    class EnableSymbolTable
    {
    public:
        SymbolTable &symbolTable() noexcept
        {
            return _symbolTable;
        }

    private:
        SymbolTable _symbolTable;
    };
}

#endif //NAIVE_PASCAL_COMPILER_SYMBOL_TABLE_HPP
