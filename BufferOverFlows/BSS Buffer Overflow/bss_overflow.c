#include <stdio.h>
#include <string.h>

// Global uninitialized buffer (BSS)
char buffer[32];

// Global uninitialized function pointer (BSS)
void (*func_ptr)();

void safe_function() {
    printf("✅ Safe function executed.\n");
}

void hacked_function() {
    printf("🚨 Hacked! Buffer overflow in BSS hijacked control flow.\n");
}

int main() {
    func_ptr = safe_function;

    printf("buffer @ %p\n", buffer);
    printf("safe_function @ %p\n", safe_function);
    printf("func_ptr @ %p\n", &func_ptr);
    printf("func_ptr-> @ %p\n", func_ptr);
    printf("hacked_function @ %p\n", hacked_function);

    FILE *f = fopen("input.bin", "rb");
    if (!f) {
        perror("input.bin");
        return 1;
    }

    // Read more than 32 bytes to overflow
    size_t n = fread(buffer, 1, 40, f);
    printf("Read %zu bytes\n", n);
    fclose(f);


    printf("\n\n buffer @ %p\n", buffer);
    printf("safe_function @ %p\n", safe_function);
    printf("func_ptr @ %p\n", &func_ptr);
    printf("func_ptr-> @ %p\n", func_ptr);
    printf("hacked_function @ %p\n", hacked_function);
    printf("Size of function pointer: %zu bytes\n", sizeof(func_ptr));

    // Call the (possibly overwritten) function pointer
    func_ptr();

    return 0;
}
