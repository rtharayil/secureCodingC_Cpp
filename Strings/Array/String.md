# String Data Type in C and C++

## Fundamental Concepts

The string data type represents a sequence of characters with specific implementation characteristics in C and C++:

### Core Definition
- A string is **a contiguous sequence of characters** terminated by (and including) the first null character (`'\0'`)
- A pointer to a string points to its initial (first) character
- String length is the number of bytes **preceding** the null character
- String value consists of the sequence of character values in order

## Memory Representation

Using the example "hello" from Figure 2.1:

```
Memory Layout:
+-----+-----+-----+-----+-----+-----+
| 'h' | 'e' | 'l' | 'l' | 'o' | '\0'|
+-----+-----+-----+-----+-----+-----+
  0x100 0x101 0x102 0x103 0x104 0x105
```

Key characteristics:
- Occupies 6 bytes (5 chars + null terminator)
- A pointer to this string would contain address `0x100`
- Length = 5 (bytes/characters before null terminator)

## Implementation as Character Arrays

Strings are implemented as arrays of characters with special properties:

```c
char str1[] = "hello";  // Array size automatically determined (6 elements)
char str2[6] = "hello"; // Explicit size including null terminator
char str3[5] = "hello"; // UNDEFINED BEHAVIOR - no room for null terminator
```

## Pointer Relationship

As with all arrays in C, strings have a close relationship with pointers:

```c
char *str_ptr = "hello";  // Pointer to string literal
char str_array[] = "world"; // Array initialization

// Equivalent ways to access first character
char first1 = *str_ptr;    // 'h'
char first2 = str_array[0]; // 'w'
```

## Common Vulnerabilities

Strings inherit all array vulnerabilities plus additional ones:

1. **Buffer Overflows**:
   ```c
   char buffer[5];
   strcpy(buffer, "hello"); // Overflow - tries to write 6 bytes
   ```

2. **Missing Null Terminator**:
   ```c
   char bad_str[3] = {'a', 'b', 'c'}; // Not a valid string - no null terminator
   ```

3. **String Literal Modification**:
   ```c
   char *ptr = "constant";
   ptr[0] = 'C'; // UNDEFINED BEHAVIOR - string literals may be in read-only memory
   ```

## C vs. C++ String Handling

| Characteristic       | C Style Strings              | C++ std::string               |
|----------------------|-----------------------------|------------------------------|
| Implementation       | Character arrays            | Class object                  |
| Length Management    | Manual (strlen)             | Automatic (length() method)   |
| Memory Management    | Manual                      | Automatic                     |
| Resizing             | Not directly supported      | Supported (append, etc.)      |
| Safety               | Prone to errors             | Bounds checking available     |
| Literal Type         | char[] or char*             | const char*                   |
| Null Terminator      | Required                    | Maintained internally         |

## Best Practices

1. Always ensure proper null termination in C strings
2. For fixed strings, prefer `const char*` to avoid modification attempts
3. When using character arrays, ensure sufficient space for null terminator
4. In C++, prefer `std::string` for most use cases
5. Use safe functions like `strncpy` instead of `strcpy` in C
6. Never assume a character array is properly null-terminated without verification

## Standard Library Functions (C)

Common `<string.h>` functions that operate on null-terminated strings:

- `strlen()` - Get string length
- `strcpy()/strncpy()` - Copy strings
- `strcat()/strncat()` - Concatenate strings
- `strcmp()/strncmp()` - Compare strings
- `strchr()` - Find character in string
- `strstr()` - Find substring

Each of these relies on the null terminator to determine string boundaries.