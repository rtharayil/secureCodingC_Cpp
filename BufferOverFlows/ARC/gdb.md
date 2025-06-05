Great question!

---

## 🧠 What is `RIP`?

### **RIP** stands for **Instruction Pointer Register** on **64-bit x86 (x86\_64)** systems.

* It holds the **memory address of the next instruction to execute**.
* When a function returns, the `ret` instruction **loads RIP from the stack**, which tells the CPU where to go next.

---

### 💡 Why `RIP` matters in buffer overflows:

In buffer overflow attacks, like ARC Injection:

1. You overflow a buffer on the **stack**.
2. You **overwrite the saved `RIP` value** (which was pushed there by the `call` instruction).
3. When the function executes `ret`, it pops your value into `RIP`.
4. Control jumps to the address you supplied — e.g., `secret()`.

That's how you hijack the program flow!

---

### 🔧 Example (C code):

```c
void secret() {
    printf("You reached secret!\n");
}

void vulnerable() {
    char buffer[64];
    gets(buffer); // Vulnerable!
}
```

### Stack before `ret`:

```
[ buffer      ]  ← 64 bytes
[ saved RBP   ]  ← base pointer (8 bytes)
[ saved RIP   ]  ← return address (8 bytes)
```

### Overwrite plan:

* Write 64 bytes to fill `buffer`
* +8 to overwrite RBP
* +8 more = overwrite `RIP` → jump to `secret()`

---

### 🔍 In GDB:

```bash
(gdb) info registers rip
rip            0x401196    secret
```

That means: the CPU will now execute code at address `0x401196` (your target).

---


