
## 🔍 The Problem: Integer Overflow in `calloc()`

### ✅ What `calloc()` does:

```c
void *calloc(size_t nmemb, size_t size);
```

* Allocates memory for an **array of `nmemb` elements**, each of **`size` bytes**.
* It returns a pointer to a block of `nmemb * size` bytes, **initialized to zero**.

---

## ⚠️ The Risk: `nmemb * size` Can Overflow

### ❌ If the product overflows `size_t`:

```c
void *p = calloc(1000000000, 1000000000);  // = 10^18 bytes
```

* `nmemb * size = 1,000,000,000 * 1,000,000,000` = `1,000,000,000,000,000,000` (1 exabyte!)
* But `size_t` (usually 64-bit or 32-bit unsigned integer) **cannot store this** → **wraps around to a small number**.

### ⛔ Result:

* `calloc()` thinks you're asking for a tiny amount of memory (due to overflow).
* You get a pointer to a small allocation.
* But your code assumes it got a **huge array**, so you write past bounds → **heap buffer overflow** → 💣 **vulnerabilities**!

---

## 💡 The CERT Rule (MEM07-C) Says:

> Always ensure that the result of `nmemb * size` is **safe**, i.e., it fits within `size_t`.

---

## ✅ How to Do That (Example Code)

```c
#include <limits.h>  // For SIZE_MAX

size_t nmemb = 1000000000;
size_t size = 1000000000;

if (nmemb != 0 && size > SIZE_MAX / nmemb) {
    // Overflow would occur!
    fprintf(stderr, "Memory allocation size would overflow!\n");
    exit(1);
}

void *ptr = calloc(nmemb, size);  // ✅ Safe
```

---

## 📌 General Rule

Before calling `calloc(nmemb, size)`, **always validate**:

```c
if (nmemb != 0 && size > SIZE_MAX / nmemb)
    // => Overflow risk
```

---

## 🧠 Real-World Example

### Heartbleed (OpenSSL, 2014)

* Vulnerability triggered by trusting user input that caused incorrect memory allocation sizes.
* Attackers could read memory they shouldn't have access to.

While not directly from `calloc()` overflow, it demonstrates **why validating memory sizes is critical**.

---

## 🛡️ Summary

| 🔍 Check                     | ✅ Why                                     |
| ---------------------------- | ----------------------------------------- |
| `nmemb * size <= SIZE_MAX`   | Prevent integer overflow                  |
| Use `SIZE_MAX / nmemb` trick | Ensures multiplication result is valid    |
| Validate before `calloc()`   | Avoid underallocation & memory corruption |


