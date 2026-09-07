#include <stdio.h>
#include <string.h>

int main(void) {
    int the_bit = 0xFFFFFFFF;   // all bits set (on 32-bit int)
    unsigned char to_print;

    memcpy(&to_print, &the_bit, 1);  // copy lowest byte

    printf("byte value: %c\n", to_print);

    return 0;
}