
# Session 3: Secure Coding Techniques

## Learning Objectives

* Learn practical secure coding patterns to avoid integer vulnerabilities
* Understand and apply type-safe programming techniques in C/C++
* Implement explicit boundary checks for safer arithmetic and memory use
* Explore and use safe integer libraries to mitigate overflow and conversion issues
* Leverage compiler flags and static analysis tools to detect potential issues early

---

## 1. Using Type-Safe Constructs

### Why Type Safety Matters

* In C/C++, implicit conversions between integer types (signed/unsigned, different sizes) can lead to unexpected behaviors and security risks.
* Using explicit types and avoiding mixing signed and unsigned types reduces bugs.

### Techniques

* Prefer using fixed-width integer types (`int32_t`, `uint64_t`) from `<stdint.h>` to ensure consistent size and behavior.
* Use `enum class` in C++ to restrict values and prevent unintended conversions.
* Avoid using raw integers for representing different kinds of data—wrap them in structs or classes.

### Example

```cpp
#include <cstdint>

struct SafeIndex {
    uint32_t value;
    explicit SafeIndex(uint32_t v) : value(v) {}
    // Add safe increment, decrement operators here
};

void process(SafeIndex idx) {
    // Prevent mixing with regular ints accidentally
    if (idx.value < 100) {
        // ...
    }
}
```

---

## 2. Explicit Boundary Checks

### Importance of Boundary Checks

* Arithmetic operations or memory indexing without bounds checking can cause overflows or buffer overruns.
* Always validate inputs and calculated sizes before usage.

### Common Patterns

* Before multiplying or adding integers used for memory allocation, check for overflow:

  ```c
  if (len > SIZE_MAX / element_size) {
      // handle overflow error
  }
  size_t total = len * element_size;
  ```
* When reading user inputs or external data, always check ranges before indexing buffers or allocating memory.

### Example: Safe Buffer Allocation

```c
#include <stdio.h>
#include <stdlib.h>

int allocate_buffer(size_t count, size_t size) {
    if (count == 0 || size == 0) return -1;
    if (count > SIZE_MAX / size) {
        // Overflow detected
        return -1;
    }
    void* ptr = malloc(count * size);
    if (!ptr) return -1;
    // Use buffer safely
    free(ptr);
    return 0;
}
```

---

## 3. Safe Integer Libraries

### Overview

* Safe integer libraries provide abstractions that automatically check for overflow, underflow, and unsafe conversions.
* They can throw exceptions or return errors on invalid operations.

### Popular Libraries

* **SafeInt (Microsoft SafeInt)**

  * Provides safe integer arithmetic by checking for overflow on each operation.
  * Supports explicit conversions and prevents unsafe casts.

* **LLVM’s CheckedInt**

  * Part of LLVM’s support libraries.
  * Offers checked arithmetic and helps avoid integer bugs during compilation and testing.

### Example Using SafeInt (C++)

```cpp
#include "SafeInt.hpp"

SafeInt<int> a = 100;
SafeInt<int> b = INT_MAX;
try {
    SafeInt<int> c = a + b;  // Throws exception on overflow
} catch (const SafeIntException&) {
    // Handle overflow error
}
```

---

## 4. Compiler Flags and Warnings

### Why Use Compiler Flags?

* Compilers can detect and warn about suspicious code patterns.
* Flags can also instrument code to catch undefined behavior at runtime.

### Key Compiler Flags

* `-Wall`
  Enables most common warnings.

* `-Wextra`
  Enables additional, useful warnings beyond `-Wall`.

* `-Wconversion`
  Warns about implicit type conversions that might change a value.

* `-Wsign-compare`
  Warns when comparing signed and unsigned integers.

* `-ftrapv`
  Generates traps for signed integer overflow on arithmetic operations.

* `-fsanitize=undefined`
  Enables Undefined Behavior Sanitizer, detects integer overflow, invalid conversions, and other UB at runtime.

### Example Compilation Command

```bash
gcc -Wall -Wextra -Wconversion -Wsign-compare -ftrapv -fsanitize=undefined -O2 myprogram.c -o myprogram
```

---

## 5. Static Analysis Tools Overview

### Purpose

* Detect bugs, including integer errors, at compile time without running the program.
* Provide detailed reports and suggestions for code improvement.

### Popular Tools

* **Coverity**

  * Commercial-grade static analysis widely used in industry.
  * Detects complex security and quality issues.

* **Cppcheck**

  * Open-source, focuses on C/C++.
  * Good for quick scans, free and easy to integrate.

* **Clang-Tidy**

  * Part of LLVM tooling.
  * Provides style checks and catches many common bugs, including integer issues.

### How to Use

* Run tools as part of CI pipelines or locally during development.
* Fix reported issues proactively to reduce runtime vulnerabilities.

### Example: Running Cppcheck

```bash
cppcheck --enable=all --inconclusive --std=c11 ./src
```

---

## Summary

| Technique                   | Purpose                                | Tools / Examples                           |
| --------------------------- | -------------------------------------- | ------------------------------------------ |
| Type-safe constructs        | Prevent implicit conversion bugs       | `int32_t`, `SafeInt`, `enum class`         |
| Explicit boundary checks    | Avoid overflow in size calculations    | Manual checks, `SIZE_MAX` guard            |
| Safe integer libraries      | Automatic overflow detection           | SafeInt, LLVM CheckedInt                   |
| Compiler flags and warnings | Detect issues during compile & runtime | `-Wall`, `-Wextra`, `-fsanitize=undefined` |
| Static analysis tools       | Early bug detection                    | Coverity, Cppcheck, Clang-Tidy             |

---

## Hands-On Exercises

1. Modify existing code to replace raw integer operations with `SafeInt` and observe behavior on overflow.
2. Add explicit boundary checks for memory allocation in a sample project.
3. Compile with all recommended compiler flags and fix all warnings.
4. Run static analysis tools on your project and review reported integer-related issues.


