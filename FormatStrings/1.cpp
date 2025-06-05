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