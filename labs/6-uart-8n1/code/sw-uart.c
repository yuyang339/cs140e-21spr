/*
 * reimplement your sw-uart code below using cycles.   helpers are in <cycle-util.h>
 */ 
#include "rpi.h"
#include "sw-uart.h"
#include "cycle-util.h"

// implement te putc/getc/readuntil.

// do this first: used timed_write to cleanup.
//  recall: time to write each bit (0 or 1) is in <uart->usec_per_bit>
void sw_uart_put8(sw_uart_t *uart, uint8_t c) {
    // use local variables to minimize any loads or stores
    int tx = uart->tx;
    uint32_t n = uart->cycle_per_bit,
             u = n,
             s = cycle_cnt_read();


    unimplemented();
}

// do this second: you can type in pi-cat to send stuff.
//      EASY BUG: if you are reading input, but you do not get here in 
//      time it will disappear.
int sw_uart_get8_timeout(sw_uart_t *uart, uint32_t timeout_usec) {
   int rx = uart->rx;

    // get start bit: timeout_usec=0 implies you return 
    // right away (used to be return never).
    while(!wait_until_usec(rx, 0, timeout_usec))
        return -1;



    unsigned s = cycle_cnt_read();  // subtract off slop?

    uint32_t u = uart->cycle_per_bit;
    unsigned n = u/2;
    unsigned c = 0;

    unimplemented();


    // NOTE: wait until the rx goes hi before returning.
    return (int)c;
}

// finish implementing this routine.  
sw_uart_t sw_uart_mk_helper(unsigned tx, unsigned rx,
        unsigned baud, unsigned cyc_per_bit) {

    // setup rx, and tx.
    unimplemented();

    cycle_cnt_init();       

    // make sure it makes sense.
    unsigned mhz = 700 * 1000 * 1000;
    unsigned derived = cyc_per_bit * baud;
    assert((mhz - baud) <= derived && derived <= (mhz + baud));
    // panic("cyc_per_bit = %d * baud = %d\n", cyc_per_bit, cyc_per_bit * baud);

    return (sw_uart_t) { 
            .tx = tx, 
            .rx = rx, 
            .baud = baud, 
            .cycle_per_bit = cyc_per_bit 
    };
}


/************************************************************
 * staff implementations.  this is a gross way to do things.
 * we need the inlined versions of the above, but don't want to 
 * have them in the main repo in case they are incorrect, since
 * you wouldn't be able to swap them.
 */


int sw_uart_get8(sw_uart_t *uart) {
    return sw_uart_get8_timeout(uart, ~0);
}


void sw_uart_putk(sw_uart_t *uart, const char *msg) {
    for(; *msg; msg++)
        sw_uart_put8(uart, *msg);
}

// returns the number of characters read.  deletes newline
int sw_uart_gets(sw_uart_t *uart, char *in, unsigned nbytes) {
    int i = 0;
    for(; i < nbytes-1; i++) {
        in[i] = sw_uart_get8(uart);
        if(in[i] == '\n')
            break;
    }
    in[++i] = 0;
    return i;
}


// don't pollute the rest of the code with all the stuff in the 
// <stdarg.h> header.
#include <stdarg.h>
#include "libc/va-printk.h"
int sw_uart_printk(sw_uart_t *uart, const char *fmt, ...) {
    char buf[460];

    va_list args;
    va_start(args, fmt);
        int sz = va_printk(buf, sizeof buf, fmt, args);
    va_end(args);
    assert(sz < sizeof buf-1);
    sw_uart_putk(uart,buf);
    return sz;
}

void sw_uart_write(sw_uart_t *uart, uint8_t *out, unsigned nbytes) {
    for(unsigned i = 0; i < nbytes; i++)
        sw_uart_put8(uart, out[i]);
}

// blocking read call.
void sw_uart_read(sw_uart_t *uart, uint8_t *out, unsigned nbytes) {
    for(unsigned i = 0; i < nbytes; i++)
        out[i] = sw_uart_get8(uart);
}


// read characters using <sw_uart_getc()> until: we do 
// not receive any characters for <timeout> usec.
//   - returns 0 (not -1) if nothing read.
int sw_uart_read_timeout(sw_uart_t *u, uint8_t *out,
                    uint32_t nbytes, uint32_t usec_timeout) {
    assert(nbytes>0);
    int i;
    for(i = 0; i < nbytes; i++) {
        int c = sw_uart_get8_timeout(u, usec_timeout);
        // timeout: done.
        if(c < 0)
            break;
        out[i] = c;
    }
    return i;
}
