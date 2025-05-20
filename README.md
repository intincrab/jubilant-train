# Tiny Compiler

A simple compiler written in C that compiles a custom language to JavaScript. This project demonstrates both fundamental compiler concepts and WebAssembly integration for running C code in browsers.

## Overview

This compiler demonstrates:

1. **Lexer**: Tokenizes the source code
2. **Parser**: Builds an abstract syntax tree (AST) from tokens
3. **Code Generator**: Outputs JavaScript code from the AST
4. **WebAssembly Integration**: Compiles C code to WebAssembly for browser execution

## Language Features

The language supports:

- Variable declarations and assignments
- Arithmetic expressions (+, -, *, /)
- Comparison operators (>, <, ==, !=, >=, <=)
- If-else statements
- Print statements

## Syntax Examples

```
// Variable assignment
x = 10;

// If statement
if (condition) {
    // statements
} else {
    // statements
}

// Print statement
print(expression);
```

## How C and WebAssembly Work Together

### C to WebAssembly Compilation Pipeline

1. **C Source Code**: We write our compiler in standard C language.

2. **Emscripten Compilation**: Emscripten (LLVM-based toolchain) compiles C code to WebAssembly:
   - First converts C to LLVM Intermediate Representation (IR)
   - Then transforms LLVM IR into WebAssembly binary format (.wasm)
   - Generates JavaScript "glue code" to load and interact with the WebAssembly module

3. **Result Files**:
   - `tiny-compiler.wasm`: Binary WebAssembly module containing our C compiler
   - `tiny-compiler.js`: JavaScript glue code that loads and instantiates the WASM module

### Browser Integration Architecture

```
┌─────────────────────────────────────────────┐
│                  BROWSER                     │
│                                             │
│  ┌─────────────┐        ┌────────────────┐  │
│  │  JavaScript │        │   WebAssembly  │  │
│  │  Interface  │◄─────►│  Module (WASM)  │  │
│  └─────────────┘        └────────────────┘  │
│         ▲                      ▲            │
│         │                      │            │
│         ▼                      │            │
│  ┌─────────────┐               │            │
│  │    HTML     │               │            │
│  │  Interface  │               │            │
│  └─────────────┘               │            │
│         ▲                      │            │
│         │                      │            │
└─────────┼──────────────────────┼────────────┘
          │                      │
          │                      │
┌─────────┼──────────────────────┼────────────┐
│         │                      │            │
│         ▼                      │            │
│  ┌─────────────┐        ┌────────────────┐  │
│  │  Input Code │        │  C Compiler    │  │
│  │ (TinyLang)  │───────►│(Compiled to   │  │
│  └─────────────┘        │   WebAssembly) │  │
│                         └────────────────┘  │
│                                │            │
│                                ▼            │
│                         ┌────────────────┐  │
│                         │  Output Code   │  │
│                         │  (JavaScript)  │  │
│                         └────────────────┘  │
│                                             │
│             COMPILER ARCHITECTURE           │
└─────────────────────────────────────────────┘
```

### Function Exports and Memory Management

1. **Exported Functions**: We define specific C functions to be callable from JavaScript:
   ```c
   // In C: Marked with EMSCRIPTEN_KEEPALIVE
   EMSCRIPTEN_KEEPALIVE
   char* compile(const char* source) {
       return compile_string(source);
   }
   
   EMSCRIPTEN_KEEPALIVE
   void free_result(char* result) {
       free_code(result);
   }
   ```

2. **Memory Management**:
   - WebAssembly has a linear memory model shared between JS and WASM
   - Strings passed between JavaScript and C require memory conversion
   - We allocate memory in C and return pointers that JavaScript can access
   - JavaScript must convert these pointers to proper JavaScript strings using `UTF8ToString`
   - Memory is manually managed with explicit free calls to prevent leaks

3. **Emscripten Configuration**:
   - We explicitly specify which functions to export with `-s EXPORTED_FUNCTIONS`
   - We specify helper methods with `-s EXPORTED_RUNTIME_METHODS`
   - Example: `UTF8ToString` is needed to convert C strings to JavaScript strings

### JavaScript Interaction Layer

In the browser, we use JavaScript to interface with our WebAssembly module:

```javascript
// Create wrapper functions using cwrap
compileFunction = Module.cwrap('compile', 'number', ['string']);
freeResultFunction = Module.cwrap('free_result', null, ['number']);

// Use the compiler
const resultPtr = compileFunction(source);
const result = Module.UTF8ToString(resultPtr);
freeResultFunction(resultPtr);
```

This architecture allows our C compiler to run at near-native speed in the browser.

## Setup and Build

### Prerequisites

- For native build: GCC or Clang
- For WebAssembly build: Emscripten SDK (emsdk)

### Building

#### Native Build

```bash
make
```

This creates the binary at `build/tiny-compiler`.

#### WebAssembly Build

```bash
# Run the setup script
./setup-wasm-manual.sh
```

This will:
1. Install Emscripten SDK if not already installed
2. Build the WebAssembly files in `public/`
3. Start a web server to serve the files

### Running

#### Native CLI

```bash
# Compile a source file
./build/tiny-compiler input.txt output.js

# Or output to console
./build/tiny-compiler input.txt
```

#### Web Interface

After building for WebAssembly, open:
```
http://localhost:8000/index-wasm.html
```

## Project Structure

- `src/` - Source code
  - `lexer.c/h` - Tokenization
  - `parser.c/h` - Parsing
  - `codegen.c/h` - Code generation
  - `main.c` - Main program with WebAssembly exports
- `public/` - Web interface
  - `index-wasm.html` - WebAssembly interface
  - `tiny-compiler.js` - Generated JavaScript glue code
  - `tiny-compiler.wasm` - Compiled WebAssembly binary
- `build/` - Native build outputs
- `examples/` - Example programs

## WebAssembly Advantages

1. **Performance**: Near-native speed compared to JavaScript
2. **Portability**: Runs in all major browsers
3. **Language Choice**: Write in C/C++ instead of JavaScript
4. **Memory Safety**: Runs in a sandboxed environment
5. **Direct Integration**: Seamless interaction with JavaScript

## License

MIT 