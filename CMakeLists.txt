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
        src/abstract_node.cpp
        src/abstract_node.hpp
        )

add_executable(npc ${SOURCE_FILES})
