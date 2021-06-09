/*************************************************************************
 * engler: cs140e: code for our simplistic user-level side we start 
 * with three system calls.
 */
#include <stdarg.h>
#include "libpix.h"

void sys_exit(int code) { 
    syscall_invoke_asm(SYS_EXIT, code);
}

void putk(const char *msg) {
    while(*msg)
        sys_putc(*msg++);
}

// simple printk that only takes %x as a format 
// we need a seperate routine since when actually at user level
// can't call kernel stuff.
void printk(const char *fmt, ...) {
    unsigned chr;
    va_list ap;

    va_start(ap, fmt);
    while((chr = *fmt++)) {
        switch(chr) {
        case '%': 
            if((chr = *fmt++) != 'x') 
                die("bad character\n");
            sys_put_hex(va_arg(ap, unsigned));
            break;
        default: sys_putc(chr);
        }
    }
    va_end(ap);
}
