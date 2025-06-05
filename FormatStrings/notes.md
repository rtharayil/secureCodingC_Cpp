
# 🧵 Stretchable Buffer Vulnerability in C++: Understanding the Hidden Danger

In secure C/C++ programming, **format strings** and **buffer sizes** often lead to subtle, dangerous bugs—especially when used together incorrectly. One such issue is called the **"Stretchable Buffer" vulnerability**, where the contents of one buffer are crafted (intentionally or unintentionally) to act as a **format string** in a second unsafe `snprintf()` or `sprintf()` call.

This article will explain the vulnerability, provide a real-world code example, and break down the mechanics of the attack in detail.

---

## 🔍 The Problem Setup: A Two-Stage Buffer

Let’s look at a simplified but vulnerable C++ example:

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>

int main() {
    char outbuf[512];
    char buffer[512];

    // User input simulating malicious payload
    const char* user = "%497d<\x3c\xd3\xff\xbf<nops><shellcode>";

    // Step 1: Safe use of snprintf, limits copy to 400 characters
    int bytes_written_to_buffer = snprintf(buffer, sizeof(buffer),
        "ERR Wrong command: %.400s", user);
    std::cout << "[DEBUG] Bytes written to buffer: " << bytes_written_to_buffer << std::endl;

    // Step 2: ❌ Vulnerable - using buffer as format string!
    int bytes_written_to_outbuf = snprintf(outbuf, sizeof(outbuf), buffer);
    std::cout << "[DEBUG] Bytes written to outbuf: " << bytes_written_to_outbuf << std::endl;

    std::cout << "Final output: " << outbuf << std::endl;
    return 0;
}
```

---

## 🔥 The Dangerous Pattern

At first glance, this code seems safe:

* The first call to `snprintf()` uses `%.400s` to safely copy only up to 400 bytes of `user` into `buffer`.
* `outbuf` is 512 bytes long, so it looks like we’ve avoided overflow.

### ❗ So what’s the problem?

In the **second call**, we do this:

```cpp
snprintf(outbuf, sizeof(outbuf), buffer);
```

We're using **user-controlled data (now inside `buffer`) as the *format string***.

---

## 🎯 Exploiting Format Strings

Let’s break it down:

### Step 1: The payload

```cpp
"%497d<\x3c\xd3\xff\xbf<nops><shellcode>"
```

The `%.400s` in the first `snprintf()` call **safely copies this into `buffer`**, so `buffer` becomes:

```
"ERR Wrong command: %497d<\x3c\xd3\xff\xbf<nops><shellcode>"
```

This is now a full **format string** with a `%497d` directive followed by binary bytes (which might be part of an address or shellcode).

---

### Step 2: The buffer is now dangerous

When we pass this string to `snprintf(outbuf, ...)`, we **interpret `%497d`** as an instruction to:

> Print 497 characters from an **argument not even passed**.

* `%d` expects an `int` from the stack.
* No argument is passed, so `snprintf()` reads a **random stack value**.
* `%497d` tells it to format this integer with **minimum width of 497 characters**.
* Combined with the rest of the string, this may write **more than 512 bytes** into `outbuf`.

### Outcome:

```
[DEBUG] Bytes written to outbuf: 538
```

You just **wrote 538 characters into a 512-byte buffer** — a classic buffer overflow.

---

## 🧠 Why Is This Called a "Stretchable Buffer"?

Because a **format string like `%Nd` can "stretch"** the number of characters `snprintf()` writes:

* `%1d` might print: `"0"` (1 byte)
* `%497d` might print: `"       0"` (497 spaces, 1 digit = 498 bytes)
* Add the rest of the literal string and BOOM — **overflow**

You’re not overflowing because of data size, but because of how you **formatted** the data.

---

## 💀 Real-World Risk

This technique is **especially dangerous** because:

* You may sanitize the input (`user`) using `%.Ns`, but still use it **later** as a format string.
* Attackers can craft format strings like `%x`, `%n`, `%s`, or `%d` to:

  * Leak memory
  * Write to arbitrary memory (via `%n`)
  * Trigger **undefined behavior**
* This kind of attack **bypasses length limits** placed during copying!

---

## ✅ How to Fix It

### 🔒 Rule: **Never treat user input as a format string**

Change this:

```cpp
snprintf(outbuf, sizeof(outbuf), buffer);
```

To this:

```cpp
snprintf(outbuf, sizeof(outbuf), "%s", buffer);
```

This **treats `buffer` as a plain string**, not a format specifier.

### Bonus: Add bounds checks

```cpp
if (bytes_written_to_outbuf >= sizeof(outbuf)) {
    std::cerr << "⚠️ Truncated output, possible format string issue\n";
}
```

---

## 📘 Summary

* **Stretchable Buffers** abuse format specifiers like `%497d` to force large outputs.
* If format strings are controlled by users, it can **bypass size checks** and lead to overflows.
* Always use format strings **explicitly** (e.g., `"%s"`) when printing user data.
* `snprintf()` is safe — but only when **used correctly**.


