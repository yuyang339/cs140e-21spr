#ifndef __LIBPIX_H__
#define __LIBPIX_H__
// support.
#include <stdint.h>
#include "syscalls.h"

#define die(x) do { putk(x); sys_exit(1); } while(0)

// invoke system call <sysno>
int syscall_invoke_asm(uint32_t sysno, ...);

#define sys_putc(x)     syscall_invoke_asm(SYS_PUTC, x)
#define sys_put_hex(x)  syscall_invoke_asm(SYS_PUT_HEX, x)
void sys_exit(int code);

// user level libc.
#include <string.h>
void putk(const char *msg);
void printk(const char *fmt, ...);

#endif
