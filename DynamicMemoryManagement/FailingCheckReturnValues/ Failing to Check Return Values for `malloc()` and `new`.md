
# 🧨 Failing to Check Return Values for `malloc()` and `new`

### (Why It's Dangerous and How to Avoid It)

---

## 🔍 Problem Summary

When memory allocation fails (due to exhaustion of system memory or address space), functions like `malloc()` in C and `new` in C++ **may return a null pointer or throw an exception**. Failing to check their result can lead to **undefined behavior**, including:

* segmentation faults,
* data corruption,
* denial of service,
* or security vulnerabilities (e.g., code execution via NULL dereference on embedded systems).

---

## 📌 Case 1: `malloc()` in C

### ❌ Bad Example – No Return Check

```c
#include <stdio.h>
#include <stdlib.h>

void process_data() {
    int *data = (int *)malloc(100 * sizeof(int));  // No check!

    data[0] = 42;  // 💥 Potential segmentation fault if malloc fails
    printf("First element: %d\n", data[0]);

    free(data);
}
```

### ✅ Safe Version – Check the Return Value

```c
void process_data() {
    int *data = (int *)malloc(100 * sizeof(int));
    if (!data) {
        perror("malloc failed");
        return;
    }

    data[0] = 42;
    printf("First element: %d\n", data[0]);

    free(data);
}
```

---

## 📌 Case 2: `new` in C++

### ❌ Bad Example – `new` Without Handling `std::bad_alloc`

```cpp
void allocateArray() {
    int* arr = new int[1000000000];  // May throw std::bad_alloc

    arr[0] = 99;  // 💥 If allocation fails, exception crashes the program
    delete[] arr;
}
```

### ✅ Safe Version – Catch `std::bad_alloc`

```cpp
#include <iostream>
#include <new>
using namespace std;

void allocateArray() {
    try {
        int* arr = new int[1000000000];
        arr[0] = 99;
        cout << "Value: " << arr[0] << endl;
        delete[] arr;
    } catch (const bad_alloc& e) {
        cerr << "Allocation failed: " << e.what() << endl;
    }
}
```

---

## 💡 Extra: `nothrow` Variant of `new`

If you **don’t want to use exceptions**, you can use the `nothrow` version:

```cpp
#include <iostream>
using namespace std;

void allocateArray() {
    int* arr = new(nothrow) int[1000000000];
    if (!arr) {
        cerr << "Allocation failed using nothrow\n";
        return;
    }

    arr[0] = 42;
    delete[] arr;
}
```

---

## ⚠️ Real-World Implications

* **Embedded systems** or **constrained IoT devices** often have very limited memory.
* Memory leaks or unchecked allocation failures can lead to **hard-to-debug crashes** in production.
* A `NULL` pointer dereference may trigger a **privilege escalation** or **denial of service** on some platforms.
* Example: [CVE-2008-0009](https://nvd.nist.gov/vuln/detail/CVE-2008-0009) – A `malloc()` failure in OpenSSL wasn’t checked, leading to a crash.

---

## 🛡️ Best Practices

| Language | Best Practice                                                                      |
| -------- | ---------------------------------------------------------------------------------- |
| C        | Always check if `malloc()` returned `NULL`.                                        |
| C++      | Use `try-catch` for `new`, or use `new(std::nothrow)` and check for `nullptr`.     |
| Embedded | Consider statically allocated buffers or memory pools instead.                     |
| Tools    | Use tools like Valgrind, ASan, or static analyzers to catch unhandled allocations. |

---

## 🧪 Exercise Idea for Hands-on Lab

Ask students to:

1. Allocate a large block of memory using `malloc()` and simulate a failure using a memory limit (`ulimit -v` on Linux).
2. Repeat using `new` in C++ and demonstrate `bad_alloc`.
3. Log whether allocation succeeded and continue or exit cleanly.


