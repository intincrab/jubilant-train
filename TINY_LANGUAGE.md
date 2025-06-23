# Tiny Language Documentation

A simple, educational programming language designed to demonstrate compiler construction concepts.

## Table of Contents

- [Overview](#overview)
- [Language Features](#language-features)
- [Syntax](#syntax)
- [Grammar](#grammar)
- [Data Types](#data-types)
- [Operators](#operators)
- [Control Flow](#control-flow)
- [Built-in Functions](#built-in-functions)
- [Examples](#examples)
- [Compilation Process](#compilation-process)
- [Error Handling](#error-handling)

## Overview

Tiny Language is a minimalist programming language that supports:
- Variable assignments
- Arithmetic operations
- Comparison operations
- Conditional statements (if-else)
- Print statements
- Comments

The language is designed to be simple enough for educational purposes while demonstrating key compiler concepts like lexical analysis, parsing, and code generation.

## Language Features

### ✅ Supported Features
- **Variables**: Dynamic assignment and usage
- **Arithmetic**: Addition, subtraction, multiplication, division
- **Comparisons**: Greater than, less than, equality, inequality
- **Control Flow**: If-else statements with block scoping
- **Output**: Print function for displaying values
- **Comments**: Single-line comments with `//`
- **Expressions**: Parenthesized expressions with proper precedence

### ❌ Not Supported (Yet)
- Function definitions
- Loops (for, while)
- Arrays or data structures
- String literals
- Boolean literals
- Multiple data types (only integers)

## Syntax

### Basic Structure

```tiny
// This is a comment
variable_name = expression;
```

### Case Sensitivity
- The language is **case-sensitive**
- Keywords must be lowercase: `if`, `else`, `print`
- Variables can use letters, numbers, and underscores

### Whitespace
- Whitespace (spaces, tabs, newlines) is ignored except for separating tokens
- Indentation is not significant (unlike Python)

## Grammar

The formal grammar in EBNF (Extended Backus-Naur Form):

```ebnf
program           ::= statement*

statement         ::= assignment_stmt
                   | if_stmt
                   | print_stmt

assignment_stmt   ::= IDENTIFIER '=' expression ';'

if_stmt           ::= 'if' '(' expression ')' '{' statement* '}' 
                      ('else' '{' statement* '}')?

print_stmt        ::= 'print' '(' expression ')' ';'

expression        ::= comparison_expr

comparison_expr   ::= arithmetic_expr (comparison_op arithmetic_expr)?

arithmetic_expr   ::= term (('+' | '-') term)*

term              ::= factor (('*' | '/') factor)*

factor            ::= NUMBER
                   | IDENTIFIER
                   | '(' expression ')'

comparison_op     ::= '>' | '<' | '==' | '!=' | '>=' | '<='

IDENTIFIER        ::= [a-zA-Z_][a-zA-Z0-9_]*
NUMBER            ::= [0-9]+
```

## Data Types

### Numbers
- **Type**: Integers only
- **Range**: Standard 32-bit signed integers (-2,147,483,648 to 2,147,483,647)
- **Literals**: Decimal numbers without quotes
- **Examples**: `42`, `0`, `-15`

### Variables
- **Declaration**: Implicit (no need to declare before use)
- **Assignment**: Using the `=` operator
- **Naming**: Must start with letter or underscore, followed by letters, numbers, or underscores
- **Examples**: `x`, `counter`, `my_variable`, `value123`

## Operators

### Arithmetic Operators
| Operator | Description | Example | Result |
|----------|-------------|---------|--------|
| `+` | Addition | `5 + 3` | `8` |
| `-` | Subtraction | `5 - 3` | `2` |
| `*` | Multiplication | `5 * 3` | `15` |
| `/` | Division | `6 / 3` | `2` |

### Comparison Operators
| Operator | Description | Example | Result |
|----------|-------------|---------|--------|
| `>` | Greater than | `5 > 3` | `true` |
| `<` | Less than | `3 < 5` | `true` |
| `>=` | Greater than or equal | `5 >= 5` | `true` |
| `<=` | Less than or equal | `3 <= 5` | `true` |
| `==` | Equal to | `5 == 5` | `true` |
| `!=` | Not equal to | `5 != 3` | `true` |

### Assignment Operator
| Operator | Description | Example |
|----------|-------------|---------|
| `=` | Assignment | `x = 10;` |

### Operator Precedence (Highest to Lowest)
1. **Parentheses**: `()`
2. **Multiplication/Division**: `*`, `/`
3. **Addition/Subtraction**: `+`, `-`
4. **Comparison**: `>`, `<`, `>=`, `<=`, `==`, `!=`

## Control Flow

### If-Else Statements

```tiny
if (condition) {
    // statements executed if condition is true
} else {
    // statements executed if condition is false
}
```

**Rules:**
- Condition must be enclosed in parentheses
- Body must be enclosed in curly braces
- `else` clause is optional
- Supports nested if statements

**Examples:**

```tiny
// Simple if
if (x > 10) {
    print(x);
}

// If-else
if (x > y) {
    print(x);
} else {
    print(y);
}

// Nested if
if (x > 0) {
    if (x > 10) {
        print(1);
    } else {
        print(2);
    }
} else {
    print(3);
}
```

## Built-in Functions

### print(expression)
- **Purpose**: Output a value to the console
- **Parameter**: Any valid expression
- **Return**: None (void)
- **Syntax**: `print(expression);`

**Examples:**
```tiny
print(42);           // Print a number
print(x);            // Print a variable
print(x + y);        // Print an expression result
print((x + y) * 2);  // Print a complex expression
```

## Examples

### Example 1: Basic Arithmetic
```tiny
// Basic arithmetic operations
a = 10;
b = 5;

sum = a + b;
difference = a - b;
product = a * b;
quotient = a / b;

print(sum);        // Output: 15
print(difference); // Output: 5
print(product);    // Output: 50
print(quotient);   // Output: 2
```

### Example 2: Conditional Logic
```tiny
// Find the maximum of two numbers
x = 25;
y = 30;

if (x > y) {
    print(x);
} else {
    print(y);
}
// Output: 30
```

### Example 3: Complex Expressions
```tiny
// Evaluate a mathematical formula
a = 5;
b = 10;
c = 3;

// Calculate: (a + b) * c - a
result = (a + b) * c - a;
print(result);  // Output: 40

// Using comparison in conditional
if (result > 30) {
    print(1);   // This will execute
} else {
    print(0);
}
```

### Example 4: Nested Conditions
```tiny
// Grade classification
score = 85;

if (score >= 90) {
    print(4);  // A grade
} else {
    if (score >= 80) {
        print(3);  // B grade - This will execute
    } else {
        if (score >= 70) {
            print(2);  // C grade
        } else {
            print(1);  // Below C
        }
    }
}
```

### Example 5: Variable Reassignment
```tiny
// Variables can be reassigned
counter = 0;
print(counter);  // Output: 0

counter = counter + 1;
print(counter);  // Output: 1

counter = counter * 5;
print(counter);  // Output: 5
```

### Example 6: Comments and Documentation
```tiny
// This program calculates the area of a rectangle
// and determines if it's a square

// Rectangle dimensions
width = 8;
height = 6;

// Calculate area
area = width * height;
print(area);  // Output: 48

// Check if it's a square
if (width == height) {
    print(1);  // It's a square
} else {
    print(0);  // It's not a square - This will execute
}
```

## Compilation Process

### 1. Lexical Analysis (Tokenization)
The source code is broken down into tokens:

```tiny
x = 10 + 5;
```

Becomes:
- `IDENTIFIER("x")`
- `ASSIGN("=")`
- `NUMBER("10")`
- `PLUS("+")`
- `NUMBER("5")`
- `SEMICOLON(";")`

### 2. Syntax Analysis (Parsing)
Tokens are organized into an Abstract Syntax Tree (AST):

```
PROGRAM
└── ASSIGN
    ├── Variable: "x"
    └── BINARY_OP: "+"
        ├── NUMBER: 10
        └── NUMBER: 5
```

### 3. Code Generation
The AST is traversed to generate JavaScript code:

```javascript
var x;
x = (10 + 5);
```

## Error Handling

### Syntax Errors
Common syntax errors and their messages:

```tiny
// Missing semicolon
x = 10  // Error: Expected ';'

// Missing closing brace
if (x > 5) {
    print(x);
// Error: Expected '}'

// Invalid operator
x = 10 & 5;  // Error: Unknown character: &
```

### Runtime Behavior
- **Division by zero**: Results in JavaScript `Infinity`
- **Undefined variables**: JavaScript `undefined` behavior
- **Type errors**: Not applicable (integers only)

## Language Limitations

### Current Limitations
1. **No string support**: Only integers are supported
2. **No loops**: No `for`, `while`, or `do-while` constructs
3. **No functions**: Cannot define custom functions
4. **No arrays**: No data structures beyond simple variables
5. **No boolean literals**: Use integers (0 for false, non-zero for true)
6. **No floating-point**: Only integer arithmetic

### Future Enhancements
- String literals and operations
- Boolean data type
- Loop constructs
- Function definitions
- Array support
- More built-in functions

## Best Practices

### Code Style
```tiny
// Good: Clear variable names
user_age = 25;
max_score = 100;

// Good: Proper spacing
result = (a + b) * c;

// Good: Comments for complex logic
if (score >= passing_grade) {
    // Student passed the exam
    print(1);
} else {
    // Student needs to retake
    print(0);
}
```

### Program Structure
```tiny
// 1. Variable declarations at the top
width = 10;
height = 5;
area = 0;

// 2. Calculations
area = width * height;

// 3. Output results
print(area);

// 4. Conditional logic
if (width == height) {
    print(1);  // Square
} else {
    print(0);  // Rectangle
}
```

## Interactive Compiler

The Tiny Language compiler includes a web-based interactive environment featuring:

- **Real-time compilation**: See JavaScript output as you type
- **AST visualization**: Interactive tree view of your program structure
- **Token analysis**: View how your code is tokenized
- **Error reporting**: Immediate feedback on syntax errors
- **Example programs**: Pre-built examples to learn from

Visit the compiler interface to experiment with the language and see how your code is processed through each compilation stage.

---

*This documentation covers Tiny Language v1.0. For updates and contributions, see the project repository.* 
