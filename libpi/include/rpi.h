/*
 * General functions we use.  These could be broken into multiple small
 * header files, but that's kind of annoying to context-switch through,
 * so we put all the main ones here.
 */
#ifndef __RPI_H__
#define __RPI_H__

#define RPI_COMPILED

// We are running without an OS, but these will get pulled from gcc's include's,
// not your laptops.
// 
// however, we don't want to do this too much, since unfortunately header files
// have a bunch of code that won't work on a pi, which can lead to problems.
#include <stddef.h>
#include <stdint.h>

void uart_init(void);

int (printk)(const char *format, ...);

// reboot after printing out a string to cause the unix my-install to shut down.
void clean_reboot(void) __attribute__((noreturn));

/*****************************************************************************
 * Low-level code: you could do in C, but these are in assembly to defeat
 * the compiler.
 */

// *(unsigned *)addr = v;
void PUT32(uint32_t addr, uint32_t v);
void put32(volatile void *addr, uint32_t v);

// *(unsigned *)addr
uint32_t GET32(uint32_t addr);
uint32_t (get32)(const volatile void *addr);

// jump to <addr>
void BRANCHTO(unsigned addr);

// a no-op routine called to defeat the compiler.
void dummy(unsigned);

#include "gpio.h"

#define nop() dummy(0)

// hack for the first couple labs: countdown 'ticks' cycles
static inline void delay(unsigned ticks) {
    while(ticks-- > 0)
        nop();
}


#ifndef RPI_UNIX
#   include "libc/assert.h"
#endif
#endif
