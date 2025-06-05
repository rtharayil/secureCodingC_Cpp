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
