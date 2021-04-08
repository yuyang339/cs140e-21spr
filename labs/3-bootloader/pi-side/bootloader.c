/*                  DO NOT MODIFY THIS CODE.
 *                  DO NOT MODIFY THIS CODE.
 *                  DO NOT MODIFY THIS CODE.
 *                  DO NOT MODIFY THIS CODE.
 *
 * engler, cs140e: simple driver for your get_code implementation,
 * which does the actual getting and loading of the code.
 *
 * much more robust than xmodem, which seems to have bugs in terms of 
 * recovery with inopportune timeouts.
 */
#include "rpi.h"
#include "staff-crc32.h"    // has the crc32 implementation.
#include "simple-boot.h"    // protocol values.

/*
 * input output definitions for get_code: this might look a bit
 * weird, but it allows us to repurpose the get_code implementation
 * for other devices later on.
 */

// blocking calls to send / receive a single byte from the uart.
#define boot_put8       uart_putc
#define boot_get8       uart_getc
// non-blocking: returns 1 if there is data, 0 otherwise.
#define boot_has_data   uart_has_data

#if 0
// note: these will lock up the machine if you lose
// data or it does not show up.
uint32_t boot_get32(void) {
	uint32_t u = boot_get8();
        u |= boot_get8() << 8;
        u |= boot_get8() << 16;
        u |= boot_get8() << 24;
	return u;
}
void boot_put32(uint32_t u) {
    boot_put8((u >> 0)  & 0xff);
    boot_put8((u >> 8)  & 0xff);
    boot_put8((u >> 16) & 0xff);
    boot_put8((u >> 24) & 0xff);
}

// returns 1 when there is data, or 0 if there was
// a timeout.
unsigned boot_has_data(unsigned timeout) {
    unsigned s = timer_get_usec();
    do {
        if(uart_has_data())
            return 1;
    // the funny subtraction is to prevent wrapping.
    } while((timer_get_usec() - s) < timeout);
    return 0;
}
#endif

#include "get-code.h"

// Simple bootloader: put all of your code here: implement steps 1,2,3,4,5,6
void notmain(void) {
    uart_init();

    long addr;
    if((addr = get_code()) < 0)
        rpi_reboot();

	// We used to have these delays to stop the unix side from getting 
    // confused.  I believe it's b/c the code we call re-initializes the 
    // uart.  Instead we now flush the hardware tx buffer.   If this
    // isn't working, put the delay back.  However, it makes it much faster
    // to test multiple programs without it.
    // delay_ms(500);
    uart_flush_tx();

    // run what we got.
    BRANCHTO(addr);

    // should not get back here, but just in case.
    rpi_reboot();
}
