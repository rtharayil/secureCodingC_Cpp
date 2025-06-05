
# 🧪 Lab Worksheet: Handling Memory Allocation Failures in C and C++

## 🎯 Objectives

By the end of this lab, participants will be able to:

* Detect and handle `malloc()` failures in C.
* Detect and handle `new` failures in C++.
* Simulate low-memory conditions.
* Avoid undefined behavior due to NULL pointer dereference.

---

## 🔧 Prerequisites

* Basic knowledge of C/C++.
* GCC or Clang installed.
* Optional: `valgrind` or `ulimit` command on Unix-like systems.

---

## 🧩 Part 1: C – Handling `malloc()` Failure

### 🔴 Task 1: Unsafe Allocation

```c
// file: unsafe_malloc.c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr = malloc(1000000000 * sizeof(int)); // Huge alloc

    arr[0] = 42;  // ❌ What happens if malloc fails?

    printf("Value: %d\n", arr[0]);
    free(arr);
    return 0;
}
```

### ✅ Task 2: Safe Allocation

```c
// file: safe_malloc.c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *arr = malloc(1000000000 * sizeof(int));

    if (!arr) {
        perror("malloc failed");
        return 1;
    }

    arr[0] = 42;
    printf("Value: %d\n", arr[0]);
    free(arr);
    return 0;
}
```

### 🧪 Experiment

Use this command before running the programs to simulate failure:

```bash
ulimit -v 102400  # Limit to 100 MB of virtual memory
```

Then run both programs. Observe behavior and compare results.

---

## 🧩 Part 2: C++ – Handling `new` Failures

### 🔴 Task 1: Unsafe `new`

```cpp
// file: unsafe_new.cpp
#include <iostream>
using namespace std;

int main() {
    int *arr = new int[1000000000];  // May throw std::bad_alloc
    arr[0] = 42;
    cout << "Value: " << arr[0] << endl;
    delete[] arr;
    return 0;
}
```

### ✅ Task 2: Safe `new` with `try-catch`

```cpp
// file: safe_new.cpp
#include <iostream>
#include <new>
using namespace std;

int main() {
    try {
        int *arr = new int[1000000000];
        arr[0] = 42;
        cout << "Value: " << arr[0] << endl;
        delete[] arr;
    } catch (const bad_alloc &e) {
        cerr << "Allocation failed: " << e.what() << endl;
    }
    return 0;
}
```

### ✅ Task 3: Safe `new` with `nothrow`

```cpp
// file: safe_new_nothrow.cpp
#include <iostream>
using namespace std;

int main() {
    int *arr = new(nothrow) int[1000000000];

    if (!arr) {
        cerr << "Allocation failed using nothrow." << endl;
        return 1;
    }

    arr[0] = 99;
    cout << "Value: " << arr[0] << endl;
    delete[] arr;
    return 0;
}
```

---

## ✅ Evaluation Criteria

| Task                         | Complete? (✓/✗) |
| ---------------------------- | --------------- |
| Unsafe `malloc` run          |                 |
| Safe `malloc` implementation |                 |
| Unsafe `new` run             |                 |
| Safe `new` with exceptions   |                 |
| Safe `new` with `nothrow`    |                 |

---

## 🧠 Discussion Questions

1. What happened when you tried to use memory returned by a failed `malloc()` or `new`?
2. What’s the difference between `new` and `new(nothrow)` in C++?
3. How would such unchecked allocations be dangerous in embedded systems?



