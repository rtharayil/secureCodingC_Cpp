
# 🧠 Session 2: Integer Representation & Risky Patterns

---

## 🎯 Learning Objectives

By the end of this session, participants will:

* Understand how integers are represented in memory using binary encoding techniques like two’s complement.
* Identify the differences between signed and unsigned integers at the bit level.
* Recognize risky coding patterns in C/C++ that can cause logic bugs or vulnerabilities.
* Learn how improper handling of integer types can lead to incorrect pointer arithmetic and unsafe buffer sizing.
* Practice secure coding practices to mitigate these risks using code examples and compiler support.

---

## 1. 🧮 Integer Representation in Memory

### 🔹 Binary Representation Basics

* Computers represent all data as binary — integers included.
* An **n-bit** integer can represent $2^n$ distinct values.

### 🔹 Unsigned Integers

* `unsigned int`, `uint32_t`, `size_t`, etc.
* Represent values from 0 to $2^n - 1$
* Example: an 8-bit `unsigned char` ranges from 0 to 255.

| Decimal | Binary (8-bit) |
| ------- | -------------- |
| 0       | 00000000       |
| 1       | 00000001       |
| 255     | 11111111       |

### 🔹 Signed Integers — Two’s Complement

Most systems use **two’s complement** to represent signed integers in C/C++. This has several advantages:

* Arithmetic is consistent (e.g., `x - y` and `x + (-y)` work the same)
* Only one representation of 0
* No special hardware for negative numbers

### ➕ How Two’s Complement Works

* The **most significant bit (MSB)** indicates the sign:

  * `0` → positive
  * `1` → negative
* For negative values:

  * Flip all bits
  * Add 1

#### Example: Represent -5 in 8-bit signed integer

1. Start with 5: `00000101`
2. Flip bits: `11111010`
3. Add 1: `11111011` → This is -5

| Decimal | Binary (8-bit) |
| ------- | -------------- |
| 0       | 00000000       |
| 1       | 00000001       |
| -1      | 11111111       |
| -2      | 11111110       |
| -128    | 10000000       |

#### ⚠️ Integer Limits

For 32-bit integers:

* `int` → −2,147,483,648 to 2,147,483,647
* `unsigned int` → 0 to 4,294,967,295

Use `limits.h` in C or `<climits>` in C++ to verify:

```c
#include <limits.h>
printf("INT_MIN = %d, INT_MAX = %d\n", INT_MIN, INT_MAX);
```

---

### 🧪 Hands-On Lab: View and Cast Representations

**Objective:** Show how the same binary bits are interpreted differently based on signedness.

```c
#include <stdio.h>

int main() {
    signed char a = -1;
    unsigned char b = (unsigned char) a;

    printf("Signed a: %d\n", a);        // -1
    printf("Unsigned b: %u\n", b);      // 255
    printf("Hex: 0x%x\n", b);           // 0xff
    return 0;
}
```

🧪 Try with different values like `-128`, `127`, `0`, `255`. Use `%x`, `%d`, `%u`.

🧰 Compile with:

```bash
gcc -Wall -Wextra -o int_repr int_repr.c
```

---

## 2. ⚠️ Common Risky Patterns in C/C++

---

### 🔹 a. `size_t` vs `int` Mismatch

#### 🧠 What’s the Problem?

* `size_t` is **unsigned**, `int` is **signed**.
* Many standard functions return `size_t`, but developers often use `int` for indexing or loops.
* Mixing them can lead to incorrect comparisons, infinite loops, or memory allocation issues.

#### 📌 Example:

```c
#include <stdlib.h>
#include <stdio.h>

int main() {
    int length = -5;
    char *buf = malloc(length); // cast to huge size_t → large allocation
    if (!buf) {
        perror("malloc failed");
        return 1;
    }
    free(buf);
    return 0;
}
```

#### 🧪 Lab Exercise

* Write code using loop `for (int i = 0; i < size_t_value; i++)`.
* Observe what happens when `size_t_value > INT_MAX` or is 0.

#### 🔧 Compiler flags

```bash
gcc -Wall -Wsign-compare -Wconversion size_t_vs_int.c
```

---

### 🔹 b. Pointer Arithmetic Mistakes

#### 🧠 Why it’s Risky

* Pointers and arrays are closely linked in C/C++.
* Integer overflows can cause pointer arithmetic to move outside valid bounds.
* May result in buffer overflows, memory corruption, or data leakage.

#### 📌 Example:

```c
#include <stdlib.h>
#include <stdio.h>

int main() {
    int index = 1000000;
    char *arr = malloc(100);
    if (!arr) return 1;

    char *p = arr + index;  // Pointer beyond allocated memory
    *p = 'X';               // Undefined behavior
    free(arr);
    return 0;
}
```

#### 🧪 Lab Exercise:

* Allocate a small buffer.
* Try pointer arithmetic with large integers.
* Run with ASan:

```bash
gcc -fsanitize=address -g pointer_arith.c -o pointer_arith
./pointer_arith
```

Observe the sanitizer's report on invalid accesses.

---

### 🔹 c. Buffer Sizing with Integer Arithmetic

#### 🧠 Why it’s Dangerous

* If `count * sizeof(type)` overflows, it results in too little memory being allocated.
* The following `memcpy` or `memset` may write beyond bounds.

#### 📌 Real-World Example:

```c
uint32_t count = user_input(); // Attacker-controlled
uint32_t size = count * sizeof(struct payload);
char *buf = malloc(size);
memcpy(buf, data, size);  // Overwrite possible if size < expected
```

#### 🧪 Lab Exercise:

* Write a function that computes buffer size using multiplication.
* Use large values for `count` (e.g., `0xFFFFFFFF`) and observe what happens.

#### ✅ Safer Approach:

```c
if (count > UINT32_MAX / sizeof(struct payload)) {
    // Prevent overflow
    return NULL;
}
size_t total = count * sizeof(struct payload);
char *buf = malloc(total);
```

#### 🔧 Compiler Flags:

```bash
gcc -Wall -fsanitize=undefined -O2 buffer_sizing.c -o buffer_sizing
```

---

## 📘 Summary of Compiler Support for Detection

| **Flag**               | **Purpose**                                   |
| ---------------------- | --------------------------------------------- |
| `-Wall`                | Enable common warnings                        |
| `-Wextra`              | Enable additional warnings                    |
| `-Wconversion`         | Warn on implicit type conversions             |
| `-Wsign-compare`       | Warn on signed/unsigned comparisons           |
| `-fsanitize=undefined` | Detect undefined behavior like overflows      |
| `-fsanitize=address`   | Detect buffer overflows via pointer issues    |
| `-ftrapv`              | Trap signed overflow (addition/mult/subtract) |

---

## ✅ Outcomes Recap

After this session, participants should be able to:

* Describe how integers are stored and interpreted in memory using binary representations.
* Distinguish between signed and unsigned integer types and their behavior.
* Identify and correct code that mixes signed/unsigned types unsafely.
* Avoid buffer sizing errors and pointer bugs due to integer miscalculations.
* Use compiler sanitizers and warnings to catch integer-related mistakes early.

