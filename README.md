# Kaleidoscope Compiler

A complete, JIT-compiling compiler for the **Kaleidoscope** language, built from
scratch on top of [LLVM](https://llvm.org/). Kaleidoscope is a small functional
language (every value is a 64-bit `double`) that is the basis of the official
LLVM front-end tutorial. This project implements the full pipeline — from raw
source text all the way to optimized native machine code executed on the fly.

## Aim of the project

The goal is to demonstrate, end to end, how a real compiler front-end and JIT
back-end fit together:

1. **Lexing** — turn source text into a stream of tokens.
2. **Parsing** — build an Abstract Syntax Tree (AST) using recursive descent
   with operator-precedence handling.
3. **Code generation** — lower the AST into [LLVM IR](https://llvm.org/docs/LangRef.html).
4. **Optimization** — run a pass pipeline (InstCombine, Reassociate, GVN,
   SimplifyCFG) over each generated function.
5. **JIT execution** — hand the optimized IR to LLVM's ORC JIT, compile it to
   native code, and run it immediately, printing the result of each top-level
   expression.

It is both a learning reference for compiler construction and a working
interactive REPL.

## What it implements

| Stage | Component | Details |
|-------|-----------|---------|
| Lexer | `src/lexer.h` | `gettok()` token reader, `tok_*` token enum, `IdentifierStr` / `NumVal` globals |
| AST | `src/ast.h` | 7 node types: Number, Variable, Binary, Call, Prototype, Function (plus If) |
| Parser | `src/parser.h` | Recursive descent + operator-precedence (`ParseExpression`, `ParseBinOpRHS`, `ParsePrototype`, `ParseDefinition`, `ParseExtern`, `ParseTopLevelExpr`) |
| Codegen | `src/codegen.cpp` / `src/codegen.h` | `codegen()` for each node — `ConstantFP`, `fadd`/`fsub`/`fmul`, `fcmp` + `uitofp`, `CreateCall`, function prototypes & bodies |
| Optimizer | `src/codegen.cpp` | `FunctionPassManager` running InstCombine, Reassociate, GVN, SimplifyCFG |
| JIT | `src/main.cpp` | ORC `LLJIT` — `addIRModule()`, `lookup()`, native call of each expression |

## Project structure

```
kaleidoscope-compiler/
├── CMakeLists.txt        # LLVM-aware CMake build
├── README.md
├── src/
│   ├── lexer.h           # tokenizer
│   ├── ast.h             # AST node definitions
│   ├── parser.h          # recursive descent parser
│   ├── codegen.h         # codegen globals & declarations
│   ├── codegen.cpp       # IR generation + optimization passes
│   └── main.cpp          # REPL driver + ORC JIT + runtime externs
└── examples/
    ├── basic.kal         # function definitions & calls
    ├── fibonacci.kal     # recursive fib with if/then/else
    └── mandelbrot.kal    # extern + nested conditionals (putchard)
```

## Prerequisites

- A C++17 compiler (Clang or GCC)
- CMake ≥ 3.13 and Make
- LLVM development libraries (≥ 14; tested with **LLVM 18**)

On Debian/Ubuntu:

```bash
sudo apt-get update -y
sudo apt-get install -y llvm-dev clang cmake make git pkg-config zlib1g-dev
llvm-config --version
```

> **Note:** Ubuntu 24.04 ships LLVM 18. If `find_package(LLVM)` doesn't locate
> it automatically, point CMake at the config explicitly with
> `-DLLVM_DIR=/usr/lib/llvm-18/cmake`.

## Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

This produces the `kaleidoscope` executable in `build/`.

## Run

The compiler reads Kaleidoscope source from **stdin**. Pipe an expression or a
file into it:

```bash
# One-liner
echo "def add(x y) x+y; add(3,4);" | ./kaleidoscope
# => Evaluated to 7.000000

# Recursive Fibonacci
echo "def fib(x) if x < 3 then 1 else fib(x-1)+fib(x-2); fib(10);" | ./kaleidoscope
# => Evaluated to 55.000000

# Run an example file
cat ../examples/fibonacci.kal | ./kaleidoscope
```

Each top-level expression is parsed, lowered to IR, optimized, JIT-compiled, and
the result is printed as `Evaluated to <value>`. The generated LLVM IR for each
function is also printed to stderr so you can see the compiler's output.

## Language features

- **Function definitions** — `def name(args) body;`
- **Function calls** — `name(arg1, arg2)`
- **Binary operators** — `+`, `-`, `*`, `<`, `>` (with operator precedence)
- **Conditionals** — `if cond then expr else expr`
- **Extern declarations** — `extern name(args);` to call host functions
- **Top-level expressions** — evaluated immediately via the JIT

### Built-in runtime functions

These are defined in the host executable and callable via `extern`:

- `putchard(x)` — prints the character with ASCII code `x`, returns 0
- `printd(x)` — prints `x` as a double followed by a newline, returns 0

## How it works (pipeline)

```
source text
   │  lexer.h        (gettok)
   ▼
 tokens
   │  parser.h       (recursive descent + precedence)
   ▼
   AST
   │  codegen.cpp    (LLVM IRBuilder)
   ▼
 LLVM IR
   │  FunctionPassManager  (InstCombine → Reassociate → GVN → SimplifyCFG)
   ▼
 optimized IR
   │  ORC LLJIT      (addIRModule + lookup)
   ▼
 native code → executed → result printed
```

## License

Provided for educational purposes, following the structure of the official LLVM
Kaleidoscope tutorial.
