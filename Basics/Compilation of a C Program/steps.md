### 🧾 Step-by-Step Compilation of a C Program: `Hello World`

Let's take a simple C program and walk through each stage of the compilation process in detail.

---

## ✅ Step 0: The Source Code

```c
// File: hello.c
#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    return 0;
}
```

---

## 🧮 Step 1: Preprocessing (`cpp`)

**Command:**

```bash
gcc -E hello.c -o hello.i
```

**What Happens:**

* The preprocessor expands all `#include` directives.
* All macros are expanded.
* Comments are removed.

**Result:** A file `hello.i` containing the expanded source code.

**Sample output in `hello.i`:**

```c
int main() {
    printf("Hello, World!\n");
    return 0;
}
// with full stdio.h expansion above
```

---

## 🧾 Step 2: Compilation to Assembly (`cc1`)

**Command:**

```bash
gcc -S hello.i -o hello.s
```

**What Happens:**

* The compiler translates the preprocessed code into assembly code.
* Syntax is checked, errors are thrown if any.
* Optimizations may be applied.

**Result:** A file `hello.s` containing the human-readable assembly code.

**Sample snippet from `hello.s`:**

```asm
movl    $.LC0, %edi
call    printf
```

---

## 🧱 Step 3: Assembling to Object Code (`as`)

**Command:**

```bash
gcc -c hello.s -o hello.o
```

**What Happens:**

* The assembler converts the assembly code into machine code (object file).
* `.o` files are not executable.

**Result:** A file `hello.o` containing binary code.

---

## 🔗 Step 4: Linking (`ld`)

**Command:**

```bash
gcc hello.o -o hello
```

**What Happens:**

* The linker combines the object file with system libraries (like `libc` which contains `printf`).
* Resolves all undefined references (e.g., where is `printf`?).

**Result:** A binary executable file `hello`.

---

## 🚀 Step 5: Running the Program

**Command:**

```bash
./hello
```

**Output:**

```text
Hello, World!
```

---

## 🔍 Summary Diagram

```text
hello.c → [Preprocessor] → hello.i
        → [Compiler]     → hello.s
        → [Assembler]    → hello.o
        → [Linker]       → hello (executable)
```


