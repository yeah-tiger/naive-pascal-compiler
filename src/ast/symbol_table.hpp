//
// Created by yzy on 6/18/18.
//

#ifndef NAIVE_PASCAL_COMPILER_SYMBOL_TABLE_HPP
#define NAIVE_PASCAL_COMPILER_SYMBOL_TABLE_HPP

#include <string>
#include <unordered_map>

namespace npc
{
    // TODO
    using Variable = std::string;
    using SymbolTable = std::unordered_map<std::string, Variable>;

    class EnableSymbolTable
    {
    public:
        SymbolTable &symbolTable()
        {
            return _symbolTable;
        }

    private:
        SymbolTable _symbolTable;
    };
}

#endif //NAIVE_PASCAL_COMPILER_SYMBOL_TABLE_HPP
