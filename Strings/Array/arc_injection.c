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