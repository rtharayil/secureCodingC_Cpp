# Session 1: Introduction to Integer Security

## Learning Objectives

* Understand different types of integer-related vulnerabilities
* Explore how C/C++ handle integers internally
* Learn how undefined behavior leads to security issues
* Analyze real-world vulnerabilities caused by integer bugs

---

## 1. Integer Overflow and Underflow

### Concept

* **Integer Overflow** occurs when an operation results in a value greater than the maximum storable value.
* **Integer Underflow** occurs when an operation results in a value lower than the minimum storable value.

### Example (C)

```c
#include <stdio.h>
#include <limits.h>

int main() {
    int max = INT_MAX;
    printf("Max: %d\n", max);
    max = max + 1;
    printf("After Overflow: %d\n", max); // wraps to INT_MIN
    return 0;
}
```

### Hands-On Lab

* Write C programs with different types (`int`, `short`, `unsigned int`) and test overflow.
* Compile with:

  ```bash
  gcc -ftrapv -fsanitize=undefined -Wall -Wextra -Wconversion overflow_test.c -o overflow_test
  ```
* Observe runtime behavior and sanitizer output.

### Relevant Compiler Flags

* `-ftrapv`: Generates traps for signed overflow on addition, subtraction, and multiplication.
* `-fsanitize=undefined`: Enables undefined behavior checks, including integer overflow.
* `-Wall -Wextra -Wconversion`: Enables common and extended warnings, including implicit conversions and potential overflows.

---

## 2. Signed/Unsigned Conversion Issues

### Concept

* Mixing signed and unsigned types leads to implicit conversions, which may change logic outcomes.

### Example (C++)

```cpp
#include <iostream>
using namespace std;

int main() {
    int signed_val = -1;
    unsigned int unsigned_val = 1;

    if (signed_val < unsigned_val) {
        cout << "signed < unsigned" << endl;
    } else {
        cout << "signed >= unsigned" << endl;
    }

    cout << "signed_val as unsigned: " << (unsigned int)signed_val << endl;
    return 0;
}
```

### Hands-On Lab

* Use comparison and array indexing scenarios mixing signed and unsigned.
* Enable warnings:

  ```bash
  g++ -Wall -Wsign-compare -Wconversion signed_unsigned.cpp -o test
  ```

### Relevant Compiler Flags

* `-Wsign-compare`: Warns when comparing signed and unsigned values.
* `-Wconversion`: Warns on implicit conversions that may alter a value.

---

## 3. Truncation and Wrap-Around Behavior

### Concept

* **Truncation**: Data is lost when converting from a larger to smaller data type.
* **Wrap-around**: Occurs in unsigned arithmetic when exceeding max value.

### Example (C)

```c
#include <stdio.h>

int main() {
    int big = 300;
    char small = big;  // truncation
    printf("Original: %d, After Truncation: %d\n", big, small);
    return 0;
}
```

### Hands-On Lab

* Test behavior with `int -> char`, `long -> short`.
* Debug with GDB to view memory layout.
* Compile with:

  ```bash
  gcc -Wall -Wconversion -fsanitize=undefined truncation_test.c -o truncation_test
  ```

---

## 4. Real-World Vulnerabilities & CVEs (Integer Specific)

### Case Studies

#### CVE-2006-1359 – ClamAV

* Integer overflow when allocating memory for a file header.
* Resulted in writing beyond allocated memory buffer.
* Vulnerability occurred because file size \* element size overflowed `int`.

#### CVE-2009-1385 – FreeType

* Integer truncation when parsing font file data.
* Miscalculated buffer size led to memory corruption.

#### CVE-2016-4484 – Grub2

* Crafted input triggered arithmetic overflow in size calculation.
* Allowed bypass of authentication in bootloader.

#### CVE-2018-8781 – GPAC (MP4Box)

* Integer underflow in size check resulted in out-of-bounds memory write.

### Example:

```c
uint32_t len = user_input;
uint32_t alloc = len * 4;  // potential overflow if len > UINT32_MAX / 4
char *buf = malloc(alloc);
memcpy(buf, src, alloc);
```

### Hands-On

* Modify the example to trigger and catch integer overflow.
* Enable flags:

  ```bash
  gcc -fsanitize=undefined -Wall -O2 integer_cve_demo.c -o cve_demo
  ```
* Use various inputs and observe sanitizer output.

---

## 5. Impact of Undefined Behavior in C/C++

### Concept

* Certain operations in C/C++ produce **undefined behavior**.
* Compiler is free to optimize UB in unpredictable ways.

### Example (C)

```c
#include <limits.h>
#include <stdio.h>

int main() {
    int x = INT_MAX;
    int y = x + 1;  // Undefined behavior
    printf("%d\n", y);
    return 0;
}
```

### Hands-On Lab

* Compile and run with:

  ```bash
  gcc -fsanitize=undefined -Wall -O0 ub_test.c -o ub_test
  gcc -fsanitize=undefined -Wall -O2 ub_test.c -o ub_test_optimized
  ```
* Compare the output and behavior under different optimization levels.

---

## Quick Recap & Quiz

### Sample Quiz Questions

1. What is the result of adding 1 to `UINT_MAX`?
2. What happens when a `signed int` is assigned a negative value but interpreted as `unsigned`?
3. What causes truncation in type conversions?
4. Why is `x + 1` undefined when `x == INT_MAX`?
5. Which CVE demonstrates an integer overflow in size allocation?

---

## Tools and Further Reading

* CERT C Secure Coding Guidelines: INT00-INT36
* Tools: UBSan, Clang-Tidy, GCC/Clang warnings
* CVE database: [https://cve.mitre.org](https://cve.mitre.org)
* **Recommended Compiler Flags**:

  * `-Wall`: Enable all common warnings
  * `-Wextra`: Enable extra warning messages
  * `-Wconversion`: Warn on type conversions that may lose data
  * `-Wsign-compare`: Warn on signed/unsigned comparisons
  * `-ftrapv`: Trap on signed overflow
  * `-fsanitize=undefined`: Catch undefined behavior at runtime

---

## Outcome

After this session, participants will:

* Understand how basic arithmetic in C/C++ can be exploited
* Use compiler features and tools to identify integer issues
* Apply secure coding techniques to avoid such bugs
