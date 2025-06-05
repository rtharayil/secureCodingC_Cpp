
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool IsPasswordOK(void)
{
    char Password[12];
    gets(Password);
    return 0 == strcmp(Password, "goodpass");
}

void AeaccesSecretData(void)
{

    printf("🔥 You reached secret!\n");

    exit(0);
}

int main(void)
{
    bool PwStatus;
    puts("Enter password:");
    PwStatus = IsPasswordOK();
    if (PwStatus == false)
    {
        puts("Access denied");
        exit(-1);
    }
    else
    {
        AeaccesSecretData();
    }
}
