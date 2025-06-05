# `signed char` and `unsigned char` in C and C++

## Fundamental Concepts

### `signed char`
- A **1-byte** signed integer type
- Typically represents values from **-128 to 127**
- Uses two's complement representation (most common implementation)
- Guaranteed by the C/C++ standards to always be exactly 1 byte in size

### `unsigned char`
- A **1-byte** unsigned integer type
- Represents values from **0 to 255**
- Also guaranteed to be exactly 1 byte
- The only type where:
  - Every bit contributes to the value (no padding bits)
  - All possible bit patterns represent valid numbers

## Memory Representation

```
signed char value = -42;
Memory layout (two's complement):
11010110

unsigned char value = 214;
Memory layout:
11010110
```

## Key Differences

| Characteristic       | `signed char`          | `unsigned char`        |
|----------------------|------------------------|------------------------|
| Value range          | -128 to 127            | 0 to 255               |
| Overflow behavior    | Undefined (wraps commonly) | Well-defined (wraps modulo 256) |
| Arithmetic           | Signed arithmetic      | Unsigned arithmetic    |
| Bit manipulation     | Sign bit affects results | Pure bit operations    |

## Common Use Cases

### `signed char`
- When you need small signed numbers
- For compatibility with certain APIs expecting signed bytes
- In DSP applications where negative values are meaningful

### `unsigned char`
1. **Raw memory manipulation**:
   ```c
   void *memcpy(void *dest, const void *src, size_t n) {
       unsigned char *d = dest;
       const unsigned char *s = src;
       while (n--) *d++ = *s++;
       return dest;
   }
   ```

2. **Byte-oriented I/O**:
   ```c
   FILE *f = fopen("data.bin", "rb");
   unsigned char buffer[1024];
   fread(buffer, 1, sizeof(buffer), f);
   ```

3. **Character handling** (when treating chars as numbers):
   ```c
   int is_ascii(unsigned char c) {
       return c <= 127;
   }
   ```

4. **Bit manipulation**:
   ```c
   unsigned char flags = 0;
   flags |= 1 << 3;  // Set bit 3
   ```

## Important Notes

1. **Plain `char` is distinct**:
   - May be signed or unsigned depending on implementation
   - Use `signed char`/`unsigned char` when you need guaranteed signedness

2. **Character literals**:
   ```c
   char c = 'A';      // Character literal (type is int in C, char in C++)
   signed char sc = 65;  // Numeric assignment
   ```

3. **Arithmetic promotions**:
   - Both types are promoted to `int` in expressions (unless `int` can't represent all values)
   - This can affect comparison results:
     ```c
     unsigned char uc = 200;
     signed char sc = -50;
     if (uc > sc) // Both promoted to int, comparison works as expected
     ```

4. **Standard guarantees**:
   - `sizeof(char) == sizeof(signed char) == sizeof(unsigned char) == 1`
   - But `1` byte isn't necessarily 8 bits (though it is on most modern systems)

## Practical Examples

### Safe range checking
```c
int safe_convert(unsigned char uc) {
    if (uc > SCHAR_MAX) {
        // Handle overflow case
        return -1;
    }
    return (signed char)uc;
}
```

### Bit rotation
```c
unsigned char rotate_left(unsigned char value, int shift) {
    return (value << shift) | (value >> (8 - shift));
}
```

### Hex dump
```c
void hexdump(const void *data, size_t size) {
    const unsigned char *bytes = data;
    for (size_t i = 0; i < size; i++) {
        printf("%02x ", bytes[i]);
    }
}
```

## Best Practices

1. Use `unsigned char` for:
   - Raw memory manipulation
   - When you need modulo-256 arithmetic
   - Bit-level operations

2. Use `signed char` for:
   - Small signed quantities
   - When interfacing with APIs expecting signed bytes

3. Avoid mixing signed and unsigned in comparisons without explicit casts

4. Remember that plain `char` may be signed or unsigned - be explicit when it matters

5. For maximum portability, don't assume 8-bit bytes (use `<limits.h>` constants like `CHAR_BIT`)