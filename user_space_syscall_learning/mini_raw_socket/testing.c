#include <stdio.h>

int main(void) {
    char val = 10; // Or char val = '\n'; (since ASCII 10 is the newline character)

    // %d prints it as a decimal (10)
    printf("Decimal: %d\n", val);

    // %X prints it as uppercase hex (0A)
    printf("Hex (uppercase): 0x%02X\n", val);

    // %x prints it as lowercase hex (0a)
    printf("Hex (lowercase): 0x%02x\n", val);

    return 0;
}