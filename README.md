# Naive Pascal Compiler

A naïve Pascal compiler for the course *Compiler Principle* at ZJU. In summary, it adopts Flex and Bison at the front end, and uses LLVM as the back end. 

## Features

- *Simple Data Types*: `boolean`, `char`, `integer`, `real`
  - string literals are also supported
- *Control Flow Statements*: if-else, case-of, while-do, repeat-until, and for loops
- *Definition Sections*: `const`, `type`, `var`, and routine sections
- *Routine* (`function` and `procedure`) definition and invocation
  - including built-in routines: `read(ln)`, `write(ln)`, `abs`, `sqrt`, `chr`, `ord`, `pred`, `succ`
- *Operators*: `+` `-` `*` `/` `div` `mod` `and` `or` `xor` `not` and comparison operators
- *Type Checking*, and *Implicit Conversion* from `integer` to `real`
- *Case Insensitivity* as per the Pascal standard

## Usage

```
USAGE: npc <option> <input.pas>
OPTION:
  -emit-llvm    Emit LLVM assembly code (.ll)
  -S            Emit assembly code (.s)
  -c            Emit object code (.o)
  -O            (Optional) do optimization
```

One of three emission options should be specified. The output file will be generated under the same directory as the input file.

- For LLVM assembly files, run `lli output.ll` to directly execute them.
- For assembly and object files, run `cc output.{s,o}` to generate executables.

## Dependencies

- CMake 3.7+
- Flex 2.5+
- Bison 3.0+
- LLVM 5.0+

## TODO

- [x] refactor header dependencies
- [ ] add more practical test programs
- [ ] explicitly fix shift/reduce conflict of if-else
- [ ] generate detailed error messages for Flex/Bison
- [ ] do error recovery in Bison and codegen
- [x] support all built-in functions and procedures
- [ ] support complex data types (string, array, record, set)
- [ ] support var parameters
- [ ] handle nested routines correctly
