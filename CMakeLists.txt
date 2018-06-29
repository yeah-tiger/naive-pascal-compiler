cmake_minimum_required(VERSION 3.7)
project(naive-pascal-compiler)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wno-deprecated-register")

find_package(BISON)
find_package(FLEX)
find_package(LLVM CONFIG)

BISON_TARGET(Parse src/parse.y ${CMAKE_BINARY_DIR}/y.tab.cpp
             DEFINES_FILE ${CMAKE_BINARY_DIR}/y.tab.h)
FLEX_TARGET(Scan src/scan.l ${CMAKE_BINARY_DIR}/lex.yy.cpp)
ADD_FLEX_BISON_DEPENDENCY(Scan Parse)

include_directories(src ${CMAKE_BINARY_DIR} ${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

file(GLOB SOURCE_FILES
    "src/main.cpp"
    "src/*/*.cpp"
    "src/*/*.hpp"
)

add_executable(npc
    ${BISON_Parse_OUTPUTS}
    ${FLEX_Scan_OUTPUTS}
    ${SOURCE_FILES}
)

llvm_map_components_to_libnames(LLVM_LIBS all)
target_link_libraries(npc ${LLVM_LIBS})
