#include <iostream>
#include <cstdio>
#include <cstring>
#include <stdio.h>

int main() {
    char outbuf[512];
    char buffer[512];

    // User input simulating malicious payload
    const char* user = "%497d<\x3c\xd3\xff\xbf<nops><shellcode>";

    // ❗️Unsafe: No buffer limit with sprintf
    int bytes_written_to_buffer = sprintf(buffer, "ERR Wrong command: %.400s", user);
    std::cout << "[DEBUG] Bytes written to buffer: " << bytes_written_to_buffer << std::endl;

    // ❌ Very dangerous: using user-controlled string as format string
    int bytes_written_to_outbuf = sprintf(outbuf, buffer);
    std::cout << "[DEBUG] Bytes written to outbuf: " << bytes_written_to_outbuf << std::endl;

    std::cout << "Final output: " << outbuf << std::endl;

    return 0;
}