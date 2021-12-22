#include "rpi.h"

static void my_putk(char *s) {
    for(; *s; s++)
        uart_putc(*s);
}

void notmain(void) {
    // hack to make sure aux is off.
    dev_barrier();
    // AUX_ENB
    PUT32(0x20215004, 0);
    dev_barrier();

    uart_init();
    my_putk("hello world\n");
}
