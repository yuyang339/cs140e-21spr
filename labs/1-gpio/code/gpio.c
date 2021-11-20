/*
 * Implement the following routines to set GPIO pins to input or output,
 * and to read (input) and write (output) them.
 *
 * DO NOT USE loads and stores directly: only use GET32 and PUT32 
 * to read and write memory.  Use the minimal number of such calls.
 *
 * See rpi.h in this directory for the definitions.
 */
#include "rpi.h"

// see broadcomm documents for magic addresses.
#define GPIO_BASE 0x20200000
static const unsigned gpio_set0  = (GPIO_BASE + 0x1C);
static const unsigned gpio_clr0  = (GPIO_BASE + 0x28);
static const unsigned gpio_lev0  = (GPIO_BASE + 0x34);
//
// Part 1 implement gpio_set_on, gpio_set_off, gpio_set_output
//
// set <pin> to be an output pin.  
//
// note: fsel0, fsel1, fsel2 are contiguous in memory, so you 
// can (and should) use array calculations!
void gpio_set_output(unsigned pin) {
    // implement this
    // use <gpio_fsel0>
    unsigned reg = (unsigned)(pin / 10);
    unsigned reg_address = GPIO_BASE + reg*4;
    unsigned ra = get32((volatile unsigned*)reg_address);
    ra &= ~(7<<((pin%10))*3);
    ra |= 0b001<<((pin%10)*3);
    put32((volatile unsigned*)reg_address, ra);
}
// put32(gpio_fsel0, get32(gpio_fsel0) & ~111b);
// set GPIO <pin> on.
void gpio_set_on(uint32_t pin) {
    // implement this
    // use <gpio_set0>
    put32((volatile uint32_t*)gpio_set0, 1<<pin);
}

// set GPIO <pin> off
void gpio_set_off(unsigned pin) {
    // implement this
    // use <gpio_clr0>
    put32((volatile uint32_t*)gpio_clr0, 1<<pin);
}

// set <pin> to <v> (v \in {0,1})
void gpio_write(unsigned pin, unsigned v) {
    if(v)
       gpio_set_on(pin);
    else
       gpio_set_off(pin);
}

//
// Part 2: implement gpio_set_input and gpio_read
//

// set <pin> to input.
void gpio_set_input(unsigned pin) {
    // implement.
    uint32_t reg = (uint32_t)(pin / 10);
    uint32_t reg_address = GPIO_BASE + reg*4;
    uint32_t ra = get32((volatile uint32_t*)reg_address);
    ra &= ~(7<<((pin%10))*3);
    ra |= 0b000<<((pin%10)*3);
    put32((volatile uint32_t*)reg_address, ra);
}

// return the value of <pin>
int gpio_read(unsigned pin) {
    uint32_t v = 0;
    v = get32((volatile uint32_t*)gpio_lev0) & (1<<pin);
    return v;
}
