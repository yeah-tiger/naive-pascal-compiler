//
// Created by Haotian on 18/6/24.
//

#ifndef NAIVE_PASCAL_COMPILER_SYM_TABLE_HPP
#define NAIVE_PASCAL_COMPILER_SYM_TABLE_HPP

#include <string>
#include <unordered_map>
#include <vector>
//#include "type_nodes.hpp"

namespace npc
{
    class Variable
    {
    public:
        Variable(std::string name, std::string type) : name(std::move(name)), type(std::move(type)) {}
        std::string name;
//        Type type;
        std::string type;
    };

    using _SymTable = std::unordered_map<std::string, Variable>;

    class SymTable {
    public:
        _SymTable &current_table() { return envs.back(); }
        Variable *find(const std::string &name, bool search_global=false) {
            for (auto e_iter = envs.rbegin(); e_iter != envs.rend(); ++e_iter) {
                auto var_iter = e_iter->find(name);
                if (var_iter != e_iter->end()) {
                    return &var_iter->second;
                }
                if (!search_global) break;
            }
            return nullptr;
        }
        size_t size() const { return envs.size(); }
        void insert(const std::string &name, const std::string &type) {
            current_table().insert({name, Variable(name, type)});
        }
        void begin_scope() { envs.emplace_back(); }
        void end_scope() { envs.pop_back(); }
    private:
        std::vector<_SymTable> envs;
    };
}

#endif //NAIVE_PASCAL_COMPILER_SYM_TABLE_HPP
