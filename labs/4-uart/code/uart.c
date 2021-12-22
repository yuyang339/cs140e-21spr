// implement:
//  void uart_init(void)
//
//  int uart_can_getc(void);
//  int uart_getc(void);
//
//  int uart_can_putc(void);
//  void uart_putc(unsigned c);
//
//  int uart_tx_is_empty(void) {
//
// see that hello world works.
//
//
#include "rpi.h"
#define UART_BASE 0x20210000
static const unsigned AUX_ENB  = (UART_BASE + 0x5004);
static const unsigned AUX_MU_IO_REG = (UART_BASE + 0x5040);
static const unsigned AUX_MU_IER_REG = (UART_BASE + 0x5044);
static const unsigned AUX_MU_IIR_REG = (UART_BASE + 0x5048);
static const unsigned AUX_MU_LCR_REG = (UART_BASE + 0x504C);
static const unsigned AUX_MU_LSR_REG = (UART_BASE + 0x5054);
static const unsigned AUX_MU_CNTL_REG = (UART_BASE + 0x5060);
static const unsigned AUX_MU_STAT_REG = (UART_BASE + 0x5064);
static const unsigned AUX_MU_BAUD = (UART_BASE + 0x5068);
// called first to setup uart to 8n1 115200  baud,
// no interrupts.
//  - you will need memory barriers, use <dev_barrier()>
//
//  later: should add an init that takes a baud rate.
void uart_init(void) {
    dev_barrier();
    // for hygien, clean up uart registers
    // disable the UART
    uint32_t ra = GET32(AUX_ENB);
    ra &= ~(1);
    ra |= (0);
    PUT32(AUX_ENB, ra);
    //  UART works in 8-bit mode
    // PUT32(AUX_MU_LCR_REG, 1);
    // set the baud rate to 115,200
    GET32(AUX_MU_BAUD);
    ra &= ~(0xffff);
    ra |= (115200);
    PUT32(AUX_MU_BAUD, ra);
    ra = GET32(AUX_ENB);
    ra &= ~(1);
    ra |= (1);
    PUT32(AUX_ENB, ra);
    dev_barrier();
}

// 1 = at least one byte on rx queue, 0 otherwise
static int uart_can_getc(void) {
    uint32_t val = GET32(AUX_MU_STAT_REG) & 0b1;
    return (int)val;
}

// returns one byte from the rx queue, if needed
// blocks until there is one.
int uart_getc(void) {
    int val = 0;
    while (!uart_can_getc()) {};
    val = (int)GET32(AUX_MU_IO_REG);
	return val;
}

// 1 = space to put at least one byte, 0 otherwise.
int uart_can_putc(void) {
    uint32_t val = GET32(AUX_MU_STAT_REG) & 0b10;
    return (int)val;
}

// put one byte on the tx queue, if needed, blocks
// until TX has space.
void uart_putc(unsigned c) {
    while(!uart_can_putc()) {};
    uint32_t val = (uint32_t)c;
    uint32_t ra = GET32(AUX_MU_IO_REG);
    ra &= ~(1<<7);
    ra |= (1<<7);
    PUT32(AUX_MU_CNTL_REG, ra);
}

// simple wrapper routines useful later.

// a maybe-more intuitive name for clients.
int uart_has_data(void) {
    return uart_can_getc();
}


// return -1 if no data, otherwise the byte.
int uart_getc_async(void) { 
    if(!uart_has_data())
        return -1;
    return uart_getc();
}

// 1 = tx queue empty, 0 = not empty.
int uart_tx_is_empty(void) {
    // unimplemented();
    uint32_t ra = GET32(AUX_MU_CNTL_REG);
    ra &= ~0b10;
    ra |= 0b10;
    PUT32(AUX_MU_CNTL_REG, ra);
    uint32_t val = GET32(AUX_MU_LSR_REG);
    return (int)(val & (1<<6));
}

void uart_flush_tx(void) {
    while(!uart_tx_is_empty())
        ;
}
