#include <iostream>
#include <cstdio>
#include <cstring>
#include <stdio.h>

int main() {
    char outbuf[512];
    char buffer[512];

    // User input simulating malicious payload
    //const char* user = "%497d<\x3c\xd3\xff\xbf<nops><shellcode>";
    const char* user = "%497d  %x %x %x %x"
   

    // Safe use of format string with limit: %.400s ensures max 400 bytes copied
    int bytes_written_to_buffer = snprintf(buffer, sizeof(buffer), "ERR Wrong command: %.400s", user);
    std::cout << "[DEBUG] Bytes written to buffer: " << bytes_written_to_buffer << std::endl;

    // ❌ Vulnerable call: using user-controlled buffer as a format string
    int bytes_written_to_outbuf = snprintf(outbuf, sizeof(outbuf), buffer);
    std::cout << "[DEBUG] Bytes written to outbuf: " << bytes_written_to_outbuf << std::endl;

    std::cout << "Final output: " << outbuf << std::endl;

    return 0;
}