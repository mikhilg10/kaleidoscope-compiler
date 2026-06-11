# Kaleidoscope Compiler

A complete compiler built with LLVM for the Kaleidoscope language.

## What it implements
- Lexer with token stream
- Recursive descent parser with operator precedence
- AST with 7 node types
- LLVM IR code generator
- Optimization pass pipeline: InstCombine, Reassociate, GVN, SimplifyCFG
- ORC JIT execution engine

## Build

```bash
mkdir build && cd build
cmake .. && make -j$(nproc)
```

## Run

```bash
echo "def fib(x) if x < 3 then 1 else fib(x-1)+fib(x-2); fib(10);" | ./kaleidoscope
```

## Language features
- Function definitions and calls
- Binary operators: + - * < >
- Conditionals: if/then/else
- Extern declarations
- Top-level expression evaluation
