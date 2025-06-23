# Tiny Compiler

A simple compiler written in C that compiles a custom language to JavaScript. This project demonstrates both fundamental compiler concepts and WebAssembly integration for running C code in browsers.

## Use Cases

This project serves multiple educational and practical purposes:

### 🎓 **Educational Applications**

#### **Computer Science Education**
- **Compiler Construction Courses**: Hands-on learning of lexical analysis, parsing, and code generation
- **Programming Language Theory**: Understanding formal grammars, syntax trees, and language design
- **Systems Programming**: Learning C programming with real-world application
- **Web Development**: Integration of low-level languages with modern web technologies

#### **Self-Learning & Research**
- **Compiler Internals**: Step-by-step visualization of compilation process
- **WebAssembly Exploration**: Practical example of C-to-WebAssembly compilation
- **Language Design**: Foundation for creating custom domain-specific languages
- **Algorithm Implementation**: Understanding parsing algorithms and tree traversal

### 💼 **Professional Development**

#### **Software Engineering Skills**
- **Cross-Platform Development**: Building tools that run both natively and in browsers
- **Performance Optimization**: Comparing native C performance vs WebAssembly
- **Memory Management**: Understanding manual memory allocation and deallocation
- **API Design**: Creating clean interfaces between C and JavaScript

#### **Technical Demonstrations**
- **Portfolio Projects**: Showcasing compiler knowledge and full-stack capabilities
- **Technical Interviews**: Demonstrating understanding of fundamental CS concepts
- **Conference Talks**: Educational tool for explaining compiler concepts
- **Code Reviews**: Reference implementation for best practices

### 🔬 **Research & Experimentation**

#### **Language Research**
- **Prototype Development**: Quick iteration on new language features
- **Syntax Experimentation**: Testing different grammatical constructs
- **Optimization Studies**: Benchmarking different compilation strategies
- **Tool Development**: Foundation for building more complex language tools

#### **WebAssembly Research**
- **Performance Analysis**: Comparing WebAssembly vs JavaScript execution
- **Integration Patterns**: Best practices for C/WebAssembly/JavaScript interaction
- **Browser Compatibility**: Testing WebAssembly features across platforms
- **Memory Model Studies**: Understanding WebAssembly's linear memory model

### 🛠️ **Practical Applications**

#### **Domain-Specific Languages (DSLs)**
- **Configuration Languages**: Template for creating custom config file processors
- **Scripting Languages**: Foundation for embedded scripting in applications
- **Template Engines**: Basis for custom template processing systems
- **Query Languages**: Starting point for specialized query processors

#### **Development Tools**
- **Code Generators**: Framework for building code generation tools
- **Transpilers**: Converting between different programming languages
- **Preprocessors**: Creating custom preprocessing tools
- **Linters**: Building syntax and semantic analysis tools

### 🌐 **Web Development**

#### **Interactive Learning Platforms**
- **Coding Tutorials**: Real-time compilation and visualization
- **Algorithm Visualization**: Interactive demonstration of compiler algorithms
- **Educational Games**: Gamified learning of programming concepts
- **Code Playgrounds**: Instant feedback environments for learning

#### **Browser-Based Tools**
- **Online IDEs**: Compilation services running entirely in the browser
- **Code Formatters**: Client-side code processing tools
- **Syntax Highlighters**: Advanced syntax analysis for editors
- **Documentation Generators**: Processing code to generate documentation

### 🎯 **Target Audiences**

#### **Students**
- Computer Science undergraduates learning compiler theory
- Self-taught programmers exploring language implementation
- Bootcamp students wanting deeper CS fundamentals
- Graduate students researching programming languages

#### **Educators**
- Professors teaching compiler construction courses
- Technical trainers demonstrating WebAssembly capabilities
- Workshop leaders explaining language design concepts
- Content creators building educational materials

#### **Professionals**
- Software engineers expanding their skill set
- Language designers prototyping new features
- Developer advocates demonstrating WebAssembly
- Technical writers documenting compiler concepts

#### **Researchers**
- Academic researchers studying programming languages
- Industry researchers exploring WebAssembly applications
- Graduate students working on compiler optimizations
- Open source contributors learning from real implementations

### 🚀 **Getting Started**

Whether you're a student learning compiler theory, a professional exploring WebAssembly, or a researcher prototyping language features, this project provides:

- **Complete Source Code**: Well-documented C implementation
- **Interactive Interface**: Real-time compilation and visualization
- **Educational Resources**: Comprehensive documentation and examples
- **Extensible Architecture**: Clean structure for adding new features

The project bridges the gap between theoretical computer science concepts and practical implementation, making it an ideal learning tool and foundation for further development.

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
- `tests/` - Test files
  - `test_parser.c` - Parser unit test
  - `Makefile` - Test compilation and execution

## Running Tests

### Parser Tests

To run the parser tests, navigate to the `tests` directory and run:

```bash
make && make test
```

This will compile and execute the parser test, which verifies:
- Basic variable assignments
- Binary operations
- Conditional statements with if/else
- Print statements

The test creates an AST from a sample program and verifies the AST structure is correct.

## WebAssembly Advantages

1. **Performance**: Near-native speed compared to JavaScript
   - WebAssembly code runs much faster than dynamically loaded JavaScript files
   - Compiled WebAssembly code can use performance optimizations like JIT (just-in-time) or AOT (ahead-of-time) compilation
   - For complex calculations, WebAssembly can be 10-100x faster than equivalent JavaScript code
   - "Compiled WebAssembly code runs much faster than dynamically loaded JavaScript files" [1]

2. **Portability**: Runs in all major browsers
   - WebAssembly is fully supported by Chrome, Firefox, Edge, and Safari
   - As a W3C standard, WebAssembly is the fourth official language of the web (alongside HTML, CSS, and JavaScript)
   - "WebAssembly is universally supported... Chrome, Firefox, Edge and Safari all execute WebAssembly files" [1]
   - Hardware-independent at both build-time and runtime, allowing for true cross-platform deployment

3. **Language Choice**: Write in C/C++ instead of JavaScript
   - Developers can use languages like C, C++, Rust, Go, and more to write web applications
   - Ideal for reusing existing codebases and leveraging language-specific features
   - "WebAssembly supports multiple programming languages... Developers can write Wasm apps in Java, Python, Rust, Golang or C++" [1]
   - This project demonstrates writing a compiler in C that runs in the browser via WebAssembly

4. **Memory Safety**: Runs in a sandboxed environment
   - The WebAssembly virtual machine validates code before execution
   - Executes within a memory-safe sandbox with limited access to system resources
   - "The virtual machine validates WebAssembly code before it runs, and executes it inside a memory-safe sandbox" [1]
   - Provides strong security guarantees compared to running native code directly

5. **Direct Integration**: Seamless interaction with JavaScript
   - WebAssembly modules can be called from JavaScript and vice versa
   - Memory can be shared between JavaScript and WebAssembly
   - Allows for incremental adoption - parts of an application can use WebAssembly while others use JavaScript
   - "WebAssembly is transparent to the client... within a web application, Wasm code is accessed through standard APIs" [1]

6. **Size Efficiency**: Compact binary format
   - WebAssembly files are significantly smaller than equivalent JavaScript files
   - "An uncompressed JavaScript file can be 100 times larger than an equivalent WebAssembly file" [1]
   - Reduces download times and improves page load performance

## References

[1] "Why WebAssembly? Top 11 Wasm benefits" - TheServerSide, https://www.theserverside.com/tip/Why-WebAssembly-Top-Wasm-benefits

[2] "Use Cases" - WebAssembly.org, https://webassembly.org/docs/use-cases/

## License

MIT 
