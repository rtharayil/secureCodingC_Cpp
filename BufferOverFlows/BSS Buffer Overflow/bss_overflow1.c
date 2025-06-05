#include <stdio.h>
#include <string.h>

char buffer[32];
void (*func_ptr)();

void safe_function() {
    printf("Safe function called.\n");
}

void hacked_function() {
    printf("Hacked! Control flow hijacked.\n");
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

    size_t n = fread(buffer, 1, 40, f);
    fclose(f);

    printf("Read %zu bytes\n", n);

    printf("\n\n buffer @ %p\n", buffer);
    printf("safe_function @ %p\n", safe_function);
    printf("func_ptr @ %p\n", &func_ptr);
    printf("func_ptr-> @ %p\n", func_ptr);
    printf("hacked_function @ %p\n", hacked_function);
    printf("Size of function pointer: %zu bytes\n", sizeof(func_ptr));
   

    func_ptr();

    return 0;
}

