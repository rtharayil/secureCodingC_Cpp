#include <stdio.h>
#include <stdlib.h>

int global_var = 10;            // Data segment
static int static_var;          // BSS segment
const char* ro_str = "Hello";   // Read-Only Data segment

void sample_function() {        // Text segment
    // Empty function for address reference
}

int main() {
    int local_var = 5;                         // Stack
    int* dynamic_var = malloc(sizeof(int));    // Heap
    *dynamic_var = 20;

    printf("Text (function):    %p\n", (void*)sample_function);
    printf("Read-Only string:   %p\n", (void*)ro_str);
    printf("Global:             %p\n", (void*)&global_var);
    printf("Static (BSS):       %p\n", (void*)&static_var);
    printf("Local:              %p\n", (void*)&local_var);
    printf("Heap:               %p\n", (void*)dynamic_var);
    getchar();

    free(dynamic_var);
    
    return 0;
}
