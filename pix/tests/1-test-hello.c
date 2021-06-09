#include "libpix.h"
#include "reg-util.h"

// fake "user program" that we use as a first step --- it's linked
// in the "kernel" address space = it's easier to interpret 
// addresses.
void notmain(void) {
    putk("hello world\n");
    printk("user: stack is roughly at %x\n", reg_get_sp());
    printk("user: cpsr=%x\n", reg_get_cpsr());
    switch(reg_get_mode()) {
    case USER_MODE: putk("USER MODE!\n"); break;
    case SUPER_MODE: putk("SUPER MODE!\n"); break;
    default: die("illegal mode\n");
    }
    sys_exit(0);
}

