# Naive Pascal Compiler

A naïve Pascal compiler for the course *Compiler Principle* at ZJU. In summary, it adopts Flex and Bison at the front end, and uses LLVM as the back end. 

## Dependencies

- CMake 3.7+
- Flex 2.5+
- Bison 3.0+
- LLVM 5.0+

## TODO List

- [x] refactor header dependencies
- [ ] add more practical test programs
- [ ] explicitly fix shift/reduce conflict of if-else
- [ ] generate detailed error messages for Flex/Bison
- [ ] support all built-in functions and procedures
- [ ] support complex data types (string, array, record, set)
- [ ] support var parameters
