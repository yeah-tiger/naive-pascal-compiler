cmake_minimum_required(VERSION 3.7)
project(naive-pascal-compiler)

set(CMAKE_CXX_STANDARD 14)

# add_custom_command(OUTPUT src/scan.g.cpp src/parse.g.cpp COMMAND make -f parser.make WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# NOTE
# plz manually run the following command:
# before using CMake

# make -f parser.make

set(SOURCE_FILES
        src/main.cpp
        src/scan.g.cpp
        src/parse.g.cpp
        src/ast/abstract_node.cpp
        src/ast/abstract_node.hpp
        src/ast/program_node.hpp src/ast/codegen_context.cpp src/ast/codegen_context.hpp src/ast/dummy_node.hpp src/ast/const_part_node.hpp src/ast/const_expr_node.hpp src/utils/ast_utils.hpp src/utils/ast.hpp src/exceptions/codegen_exception.hpp)

add_executable(npc ${SOURCE_FILES})
