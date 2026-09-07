# C Without Any Headers

> Everything the C language itself provides — no standard library needed.

---

## Table of Contents

- [Data Types](#data-types)
- [Operators](#operators)
- [Control Flow](#control-flow)
- [Functions](#functions)
- [Declarations & Scope](#declarations--scope)
- [Compound Literals & Designated Initializers](#compound-literals--designated-initializers-c99)
- [Preprocessor](#preprocessor)
- [What You Cannot Do Without Headers](#what-you-absolutely-cannot-do-without-headers)

---

## Data Types

### Primitive Types
Built directly into the language — no header needed.

| Type | Notes |
|------|-------|
| `char` | Smallest addressable unit |
| `short`, `int`, `long`, `long long` | Integer widths |
| `unsigned` variants | All integer types have unsigned versions |
| `float`, `double`, `long double` | Floating point |
| `void` | Absence of type |
| `_Bool` | C99 — 0 or 1 |
| `_Complex`, `_Imaginary` | C99 — compiler support varies |

### Derived Types

```c
int *p;                        // pointer
int arr[10];                   // array
struct { int x; float y; };    // struct
union  { int i; float f; };    // union
enum Color { RED, GREEN, BLUE }; // enum
int (*fn)(int, double);        // function pointer
```

### Type Qualifiers

| Qualifier | Purpose |
|-----------|---------|
| `const` | Value cannot be modified |
| `volatile` | Prevents compiler optimization (e.g. memory-mapped I/O) |
| `restrict` | C99 — pointer aliasing hint for optimizer |

### Storage Classes

| Class | Behaviour |
|-------|-----------|
| `auto` | Default for local variables (stack) |
| `register` | Hint to keep in CPU register |
| `static` | File scope or persists across function calls |
| `extern` | Declaration only — defined elsewhere |

---

## Operators

### Arithmetic
| Operator | Operation |
|----------|-----------|
| `+` `-` `*` `/` | Basic arithmetic |
| `%` | Modulo (integers only) |

### Bitwise
| Operator | Operation |
|----------|-----------|
| `&` | AND |
| `\|` | OR |
| `^` | XOR |
| `~` | NOT (bitwise complement) |
| `<<` `>>` | Left / right shift |

### Logical
| Operator | Operation |
|----------|-----------|
| `&&` | Logical AND (short-circuit) |
| `\|\|` | Logical OR (short-circuit) |
| `!` | Logical NOT |

### Comparison
`==`  `!=`  `<`  `>`  `<=`  `>=`

### Assignment
`=`  `+=`  `-=`  `*=`  `/=`  `%=`  `&=`  `|=`  `^=`  `<<=`  `>>=`

### Increment / Decrement
```c
++x;   x++;   // prefix and postfix, both supported
--x;   x--;
```

### Pointer Operators
| Operator | Meaning |
|----------|---------|
| `*ptr` | Dereference |
| `&var` | Address-of |
| `ptr->member` | Member access via pointer |
| `var.member` | Member access directly |

### Other Operators
| Operator | Notes |
|----------|-------|
| `sizeof(type)` / `sizeof expr` | Size in bytes — resolved at **compile time** |
| `(type) expr` | Explicit cast |
| `cond ? a : b` | Ternary |
| `a, b` | Comma — evaluates both, yields right |
| `arr[i]` | Subscript — sugar for `*(arr + i)` |
| `fn(args)` | Function call |

---

## Control Flow

```c
// Conditionals
if (cond) { } else if (cond) { } else { }

switch (expr) {
    case 1:
        /* ... */
        break;
    default:
        /* ... */
}

// Loops
while (cond) { }

do { } while (cond);

for (init; cond; step) { }

// Jumps
goto label;
label:

break;       // exit loop or switch
continue;    // skip to next iteration
return expr; // return from function
```

> [!NOTE]
> `goto` is valid C — commonly used for cleanup chains in low-level/kernel code (e.g. Linux kernel).

---

## Functions

```c
// Basic definition
int add(int a, int b) {
    return a + b;
}

// Variadic syntax (va_list requires <stdarg.h> to *use*, but the ... syntax is valid)
int foo(int n, ...);

// Recursion — fully supported
int factorial(int n) {
    return n <= 1 ? 1 : n * factorial(n - 1);
}

// Function pointer
int (*op)(int, int) = add;
int result = op(3, 4);   // calls add(3, 4)
```

---

## Declarations & Scope

```c
int x;                   // tentative definition (zero-initialized at file scope)
extern int y;            // declaration only — defined in another translation unit
static int count = 0;    // persists across calls (or: file-scope linkage)
const int MAX = 10;      // read-only
int arr[] = {1, 2, 3};  // size deduced from initializer
```

### Scope Rules

| Scope | Where |
|-------|-------|
| Block scope | Inside `{ }` |
| Function scope | Labels (`goto` targets) |
| File scope | Outside all functions |
| Function prototype scope | Parameter names in prototype |

---

## Compound Literals & Designated Initializers (C99)

```c
// Designated initializers — init by field/index name
struct Point p = { .x = 1, .y = 2 };
int arr[5] = { [2] = 99 };          // arr = {0, 0, 99, 0, 0}

// Compound literal — unnamed object with a given type
(struct Point){ .x = 3, .y = 4 };   // useful as a function argument
```

---

## Preprocessor

The preprocessor runs before compilation — no headers needed for any of this.

```c
// Object-like macro
#define MAX 100

// Function-like macro
#define SQ(x) ((x) * (x))

// Conditional compilation
#ifdef DEBUG
    /* debug-only code */
#endif

#ifndef GUARD_H
#define GUARD_H
    /* header guard body */
#endif

#if PLATFORM == 1
    /* ... */
#elif PLATFORM == 2
    /* ... */
#else
    /* ... */
#endif

// Undefine
#undef MAX

// Compiler directives
#pragma once
#error "This should not compile"
```

### Predefined Macros

| Macro | Expands to |
|-------|-----------|
| `__FILE__` | Current filename (string literal) |
| `__LINE__` | Current line number (integer) |
| `__DATE__` | Compilation date (string literal) |
| `__TIME__` | Compilation time (string literal) |
| `__func__` | Current function name — C99 (not a macro, but a predefined identifier) |

---

## What You Absolutely Cannot Do Without Headers

| Capability | Header Required |
|------------|----------------|
| `printf`, `scanf`, file I/O | `<stdio.h>` |
| `malloc`, `free`, `exit` | `<stdlib.h>` |
| `strlen`, `memcpy`, `memset` | `<string.h>` |
| `sin`, `cos`, `sqrt`, `pow` | `<math.h>` |
| `int8_t`, `uint64_t` | `<stdint.h>` |
| `bool`, `true`, `false` | `<stdbool.h>` |
| `NULL` (as macro) | any standard header |
| `size_t`, `ptrdiff_t` | `<stddef.h>` |
| `va_list`, `va_arg` | `<stdarg.h>` |
| `assert()` | `<assert.h>` |

> [!TIP]
> `NULL` is just `(void*)0` or `0` — you can `#define` it yourself.
> `true`/`false` are just `1`/`0`.
> `size_t` is `unsigned long` on most 64-bit platforms — the *type* exists, only the *name* needs a header.

---

## TL;DR

> **Everything that is *syntax* is free.**
> Everything that is a *function call* or a *named type alias* lives in a library.

```c
// This is valid C with zero headers:
int main(void) {
    int x = 42;
    int arr[5] = { [4] = x };
    int *p = arr;
    for (int i = 0; i < 5; i++) {
        *(p + i) *= 2;
    }
    return 0;
}
```
