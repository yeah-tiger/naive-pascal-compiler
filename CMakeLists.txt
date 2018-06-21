cmake_minimum_required(VERSION 3.7)
project(naive-pascal-compiler)

set(CMAKE_CXX_STANDARD 14)

add_custom_target(
        LexYaccMakefile
        COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR} && make -f parser.make
)

set(SOURCE_FILES
        src/main.cpp
        src/scan.g.cpp
        src/parse.g.cpp
        src/ast/abstract_node.hpp
        src/ast/program_node.hpp src/ast/codegen_context.cpp src/ast/codegen_context.hpp src/ast/dummy_node.hpp src/ast/const_part_node.hpp src/ast/const_expr_node.hpp src/utils/ast_utils.hpp src/utils/ast.hpp src/exceptions/codegen_exception.hpp src/ast/const_value_nodes.hpp src/ast/name_node.hpp src/ast/type_nodes.hpp src/ast/stmt_nodes.hpp src/ast/routine_nodes.hpp src/ast/parameter_nodes.hpp src/ast/symbol_table.hpp src/ast/abstract_expr_node.hpp src/ast/expr_nodes.hpp)

add_executable(npc ${SOURCE_FILES})
add_dependencies(npc LexYaccMakefile)
